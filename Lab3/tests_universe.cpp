// tests_universe.cpp
//
// Reponses aux questions 7 a 10 du Lab 3 (benchmarks de la classe Univers).
//
//   Q7  : performance de l'insertion des particules dans le conteneur.
//   Q8  : performance du calcul des interactions, pour N = (2^k)^3, k = 3..7.
//   Q9  : une modification simple qui divise le temps de calcul par 2.
//   Q10 : d'autres simplifications possibles.
//
// Chaque section imprime ses mesures ET explique, en commentaire juste
// au-dessus du code concerne, l'optimisation qu'elle illustre.

#include "univers.hpp"
#include "Particule.hpp"
#include "vector.hpp"

#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

#ifdef _OPENMP
#include <omp.h>
#endif

using Clock = std::chrono::steady_clock;

// Mesure en secondes le temps d'execution de l'appelable f().
template <typename F>
double chronometrer(F&& f) {
    auto debut = Clock::now();
    f();
    auto fin = Clock::now();
    return std::chrono::duration<double>(fin - debut).count();
}

// =====================================================================
// Q9 : plusieurs implementations du calcul des interactions, a comparer.
// =====================================================================

// ---------------------------------------------------------------------
// Version NAIVE (point de depart, celle qu'on ecrit "sans reflechir") :
// on parcourt TOUTES les paires ORDONNEES (i, j) avec i != j.
// Chaque interaction physique entre deux particules est donc calculee
// DEUX fois : une fois en tant que F_ij (quand on traite i), une fois en
// tant que F_ji (quand on traite j). Or F_ji = -F_ij (troisieme loi de
// Newton, action-reaction) : la moitie du travail est redondante.
//
// Complexite : N * (N-1) evaluations de force (sqrt + division + 3 mults).
// ---------------------------------------------------------------------
void calculerForcesNaive(std::vector<Particule>& particules) {
    for (auto& p : particules) {
        p.setForce(Vector(0, 0, 0));
    }

    for (std::size_t i = 0; i < particules.size(); ++i) {
        Vector force_i(0, 0, 0);
        for (std::size_t j = 0; j < particules.size(); ++j) {
            if (i == j) continue;

            Vector d = particules[j].getPosition() - particules[i].getPosition();
            double r = d.norm();
            double F = particules[i].getMasse() * particules[j].getMasse() / (r * r * r);

            force_i += d * F;
        }
        particules[i].setForce(force_i);
    }
}

// ---------------------------------------------------------------------
// REPONSE A LA QUESTION 9 : version SYMETRIQUE.
//
// On exploite la troisieme loi de Newton (F_ij = -F_ji) : il suffit de
// calculer chaque PAIRE non ordonnee {i, j} une seule fois (en imposant
// j > i), puis d'ajouter la contribution aux DEUX particules en meme
// temps (+d*F a la particule i, -d*F a la particule j).
//
// Complexite : N * (N-1) / 2 evaluations de force, soit exactement 2
// fois moins que la version naive -> le temps de calcul est divise par 2.
// C'est la version utilisee par Univers::calculerForces().
// ---------------------------------------------------------------------
void calculerForcesSymetrique(std::vector<Particule>& particules) {
    for (auto& p : particules) {
        p.setForce(Vector(0, 0, 0));
    }

    for (std::size_t i = 0; i < particules.size(); ++i) {
        for (std::size_t j = i + 1; j < particules.size(); ++j) {
            Vector d = particules[j].getPosition() - particules[i].getPosition();
            double r = d.norm();
            double F = particules[i].getMasse() * particules[j].getMasse() / (r * r * r);

            particules[i].setForce(particules[i].getForce() + d * F);
            particules[j].setForce(particules[j].getForce() - d * F); // F_ji = -F_ij
        }
    }
}

// =====================================================================
// Q10 : autres simplifications possibles (deux sont demontrees ici).
// =====================================================================

// ---------------------------------------------------------------------
// SIMPLIFICATION A : parallelisation (OpenMP).
//
// La version symetrique (Q9) est difficile a paralleliser telle quelle :
// a l'iteration i, on ecrit a la fois dans force[i] ET dans force[j>i],
// donc deux threads traitant des i differents peuvent ecrire dans le
// meme force[j] en meme temps (data race) -> il faudrait des sections
// atomiques/critiques, couteuses.
//
// La version NAIVE, elle, n'ecrit que dans force_i (variable locale a
// l'iteration i) : les iterations de la boucle externe sont totalement
// INDEPENDANTES. On peut donc la paralleliser sans aucune synchronisation
// avec un simple "#pragma omp parallel for".
//
// Compromis : 2x plus de travail sequentiel que la version symetrique,
// mais parfaitement parallelisable -> avec P coeurs, le gain peut aller
// jusqu'a un facteur P/2 par rapport a la version symetrique sequentielle
// (et jusqu'a P par rapport a la version naive sequentielle).
// Si le code est compile sans -fopenmp, _OPENMP n'est pas defini et la
// fonction se comporte comme la version naive sequentielle (aucun risque).
// ---------------------------------------------------------------------
void calculerForcesParallele(std::vector<Particule>& particules) {
    for (auto& p : particules) {
        p.setForce(Vector(0, 0, 0));
    }

    const int n = static_cast<int>(particules.size());

#ifdef _OPENMP
    #pragma omp parallel for schedule(static)
#endif
    for (int i = 0; i < n; ++i) {
        Vector force_i(0, 0, 0);
        for (int j = 0; j < n; ++j) {
            if (i == j) continue;

            Vector d = particules[j].getPosition() - particules[i].getPosition();
            double r = d.norm();
            double F = particules[i].getMasse() * particules[j].getMasse() / (r * r * r);

            force_i += d * F;
        }
        particules[i].setForce(force_i);
    }
}

// ---------------------------------------------------------------------
// SIMPLIFICATION B : rayon de coupure, pour eviter les sqrt inutiles.
//
// std::sqrt (appele dans Vector::norm()) est largement l'operation la
// plus couteuse de la boucle, bien plus qu'une multiplication. On peut
// l'eviter pour les paires trop eloignees pour avoir une influence
// significative : on compare d'abord le CARRE de la distance (un simple
// produit scalaire d*d, sans sqrt) au carre d'un rayon de coupure, et on
// ne calcule sqrt() que pour les paires retenues.
//
// Attention : contrairement a Q9 (exacte) et a la parallelisation
// (exacte), ceci introduit une APPROXIMATION PHYSIQUE : les interactions
// lointaines sont negligees. A n'utiliser que si le probleme le permet
// (potentiel qui decroit vite, particules suffisamment dispersees...).
// ---------------------------------------------------------------------
void calculerForcesAvecCoupure(std::vector<Particule>& particules, double rayonCoupure) {
    for (auto& p : particules) {
        p.setForce(Vector(0, 0, 0));
    }

    const double r2max = rayonCoupure * rayonCoupure;

    for (std::size_t i = 0; i < particules.size(); ++i) {
        for (std::size_t j = i + 1; j < particules.size(); ++j) {
            Vector d = particules[j].getPosition() - particules[i].getPosition();
            double r2 = d * d; // produit scalaire = norme au carre, SANS sqrt
            if (r2 > r2max) continue; // paire ignoree : aucun sqrt calcule

            double r = std::sqrt(r2);
            double F = particules[i].getMasse() * particules[j].getMasse() / (r * r * r);

            particules[i].setForce(particules[i].getForce() + d * F);
            particules[j].setForce(particules[j].getForce() - d * F);
        }
    }
}

// =====================================================================
// Programme principal : Q7, Q8, Q9, Q10
// =====================================================================

int main() {
    std::cout << std::fixed << std::setprecision(4);

    // -----------------------------------------------------------------
    // Question 7 : performance de l'implementation ET du conteneur en
    // insertion.
    //
    // On compare deux manieres d'obtenir le meme Univers rempli de N
    // particules :
    //   - "sans reserve"  : on part d'un Univers vide et on appelle
    //     ajouterParticule() (= std::vector::push_back) N fois. Le
    //     vecteur ne connait pas la taille finale a l'avance : il double
    //     sa capacite a chaque depassement, ce qui provoque O(log N)
    //     reallocations, chacune copiant/deplacant TOUS les elements deja
    //     inseres.
    //   - "avec reserve"  : Univers::genererUniforme() appelle
    //     particules.reserve(n) avant la boucle (voir univers.hpp) : la
    //     memoire finale est allouee UNE SEULE FOIS, donc aucune
    //     reallocation ni recopie pendant l'insertion.
    //
    // C'est exactement la technique derriere la reponse a la Question 9
    // appliquee cette fois au conteneur plutot qu'au calcul des forces :
    // supprimer du travail redondant (ici, des recopies memoire inutiles).
    // -----------------------------------------------------------------
    std::cout << "=== Question 7 : insertion (avec vs sans reserve) ===\n";
    std::cout << std::setw(12) << "N"
              << std::setw(18) << "sans reserve (s)"
              << std::setw(18) << "avec reserve (s)" << "\n";

    for (int k = 3; k <= 7; ++k) {
        int n = static_cast<int>(std::pow(2, k));
        int nombreParticules = n * n * n;

        double tempsSansReserve = chronometrer([&]() {
            Univers u(3);
            for (int id = 0; id < nombreParticules; ++id) {
                u.ajouterParticule(Particule(0, 0, 0, 0, 0, 0, 1.0, id, 0, 0, 0, 0));
            }
        });

        double tempsAvecReserve = chronometrer([&]() {
            Univers u(3);
            u.genererUniforme(nombreParticules, Vector(0, 0, 0), 1.0);
        });

        std::cout << std::setw(12) << nombreParticules
                  << std::setw(18) << tempsSansReserve
                  << std::setw(18) << tempsAvecReserve << "\n";
    }
    std::cout << "\n";

    // -----------------------------------------------------------------
    // Question 8 : performance du calcul des interactions, N = (2^k)^3.
    //
    // calculerForces() est en O(N^2) : chaque particule interagit avec
    // toutes les autres. Au-dela de quelques dizaines de milliers de
    // particules, le temps de calcul devient prohibitif (un N deux fois
    // plus grand multiplie le temps par quatre). On le mesure jusqu'a la
    // taille ou le temps reste raisonnable pour ce test, et on EXPLIQUE
    // pourquoi les plus grandes valeurs de k sont desactivees par defaut
    // -- c'est precisement le probleme que les questions 9 et 10 adressent.
    // -----------------------------------------------------------------
    std::cout << "=== Question 8 : calcul des interactions, N = (2^k)^3 ===\n";
    std::cout << std::setw(12) << "N" << std::setw(18) << "temps (s)" << "\n";

    // Au-dela de ce nombre de PAIRES (N*(N-1)/2), le O(N^2) devient trop
    // lent pour un simple test (des heures pour k = 6 ou 7 avec
    // calculerForces() seul). On le signale au lieu de bloquer le test.
    const double maxPairesRaisonnable = 6e8;

    for (int k = 3; k <= 7; ++k) {
        int n = static_cast<int>(std::pow(2, k));
        long long nombreParticules = static_cast<long long>(n) * n * n;
        double nbPaires = 0.5 * static_cast<double>(nombreParticules) * (nombreParticules - 1);

        if (nbPaires > maxPairesRaisonnable) {
            std::cout << std::setw(12) << nombreParticules
                      << "  ignore (O(N^2) -> " << static_cast<long long>(nbPaires)
                      << " paires, trop lent sans les optimisations Q9/Q10)\n";
            continue;
        }

        Univers u(3);
        u.genererUniforme(static_cast<int>(nombreParticules), Vector(0, 0, 0), 1.0);

        double temps = chronometrer([&]() { u.calculerForces(); });

        std::cout << std::setw(12) << nombreParticules
                  << std::setw(18) << temps << "\n";
    }
    std::cout << "\n";

    // -----------------------------------------------------------------
    // Question 9 : naive vs symetrique -> facteur 2 attendu.
    // -----------------------------------------------------------------
    std::cout << "=== Question 9 : naive vs symetrique (facteur ~2 attendu) ===\n";

    const int nTest = 32 * 32 * 32; // (2^5)^3, assez grand pour mesurer, assez petit pour rester rapide

    Univers modele(3);
    modele.genererUniforme(nTest, Vector(0, 0, 0), 1.0);
    std::vector<Particule> baseParticules(modele.getParticules().begin(), modele.getParticules().end());

    std::vector<Particule> copieNaive = baseParticules;
    double tempsNaif = chronometrer([&]() { calculerForcesNaive(copieNaive); });

    std::vector<Particule> copieSymetrique = baseParticules;
    double tempsSymetrique = chronometrer([&]() { calculerForcesSymetrique(copieSymetrique); });

    std::cout << "N = " << nTest << "\n";
    std::cout << "  naive       : " << tempsNaif << " s\n";
    std::cout << "  symetrique  : " << tempsSymetrique << " s\n";
    std::cout << "  acceleration: x" << (tempsNaif / tempsSymetrique) << "\n\n";

    // -----------------------------------------------------------------
    // Question 10 : autres simplifications (parallelisation + coupure).
    // -----------------------------------------------------------------
    std::cout << "=== Question 10 : autres simplifications ===\n";

    std::vector<Particule> copieParallele = baseParticules;
    double tempsParallele = chronometrer([&]() { calculerForcesParallele(copieParallele); });

#ifdef _OPENMP
    std::cout << "Parallelisation OpenMP (" << omp_get_max_threads() << " threads) :\n";
#else
    std::cout << "Parallelisation OpenMP (non activee a la compilation -> execution sequentielle) :\n";
#endif
    std::cout << "  naive (1 thread)     : " << tempsNaif << " s\n";
    std::cout << "  naive parallelisee   : " << tempsParallele << " s\n";
    std::cout << "  acceleration         : x" << (tempsNaif / tempsParallele) << "\n\n";

    double rayonCoupure = 0.3; // sur un cube unite [0,1]^3, ignore les paires eloignees
    std::vector<Particule> copieCoupure = baseParticules;
    double tempsCoupure = chronometrer([&]() { calculerForcesAvecCoupure(copieCoupure, rayonCoupure); });

    std::cout << "Rayon de coupure (approx. physique, rayon = " << rayonCoupure << ") :\n";
    std::cout << "  symetrique sans coupure : " << tempsSymetrique << " s\n";
    std::cout << "  symetrique avec coupure : " << tempsCoupure << " s\n";
    std::cout << "  acceleration            : x" << (tempsSymetrique / tempsCoupure) << "\n\n";

    // -----------------------------------------------------------------
    // Pistes supplementaires pour la Question 10, non codees ici mais
    // pertinentes pour aller plus loin que le O(N^2) :
    //
    //   - Partitionnement spatial (grille uniforme / cell lists) : ne
    //     tester les interactions qu'entre particules de cellules
    //     voisines -> ramene le cout a ~O(N) si la densite est uniforme.
    //   - Algorithmes en arbre (Barnes-Hut, O(N log N)) : regrouper les
    //     particules lointaines et les traiter comme une seule masse
    //     equivalente (approximation controlee par un parametre theta).
    //   - Vectorisation SIMD : compiler avec -O3 -march=native et
    //     organiser les donnees en SoA (tableaux x[], y[], z[] separes
    //     plutot que des Vector) pour que le compilateur auto-vectorise
    //     la boucle interne.
    //   - Precision reduite (float au lieu de double) si la precision
    //     physique requise le permet : deux fois moins de donnees a
    //     deplacer, operations SIMD deux fois plus larges.
    //   - Potentiel adouci (softening) : remplacer r^3 par (r^2+eps^2)^1.5
    //     pour eviter les forces infinies quand deux particules se
    //     rapprochent trop (ameliore la stabilite, pas directement la
    //     vitesse, mais evite de devoir reduire dt pres des collisions).
    // -----------------------------------------------------------------

    return 0;
}
