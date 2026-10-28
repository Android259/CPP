// Lab 3, Q6-Q10 : un Univers de (2^k)^3 particules reparties sur une grille
// reguliere dans le cube unitaire, puis le cout de sa construction (Q7) et
// celui du calcul des interactions (Q8).
//
// Q8 : le temps est en O(n^2).
// Mesures : 0.001s pour n = 512,
// 0.048s pour n = 4096,
// 3.04s pour n = 32768,
// soit un facteur ~64 a chaque passage de k a k+1 (n x 8, donc paires x 64).
//
// Q9 : car F_ji = -F_ij, on ne parcourt que les paires i<j en accumulant la
// force dans les deux particules a la fois. Il y a deux fois moins de paires,
// mais chacune coute un peu plus cher (deux mises a jour au lieu d'une).
//
// Q10 : ignorer les paires au-dela de rayonCoupure, ce qui mene au decoupage en
// cellules du Lab 4 et fait passer le cout de O(n^2) a O(n). Mettre en cache ce
// qui ne change pas (1/masse, sigma^2, rayonCoupure^2, taille des cellules). Ne plus
// allouer dans la boucle en temps (tampon de forces reutilise, std::array pour
// les coordonnees de cellule). Mettre les cellules a jour de maniere
// incrementale au lieu de tout reconstruire.

#include "../include/Univers.hpp"
#include <chrono>
#include <cstdio>
#include <vector>

double secondesDepuis(std::chrono::steady_clock::time_point debut) {
    return std::chrono::duration<double>(std::chrono::steady_clock::now() - debut).count();
}

// Univers de (2^k)^3 particules placees sur une grille reguliere du cube
// unitaire. Les extremites 0 et 1 sont incluses, d'ou le pas 1/(cote - 1).
Univers genererCube(unsigned k) {
    const unsigned cote = 1u << k;
    const unsigned n = cote*cote*cote;

    // Boite unitaire : L = 1 dans les trois directions.
    Univers univers(3, static_cast<int>(n), 1.0, Vector(0, 0, 0),
                    std::vector<double>{1.0, 1.0, 1.0}, 1.0, 1.0, 1.0);

    const double pas = 1.0/(cote - 1);
    std::size_t indice = 0;
    for (unsigned i = 0; i < cote; ++i) {
        for (unsigned j = 0; j < cote; ++j) {
            for (unsigned l = 0; l < cote; ++l) {
                univers.setPosition(indice, Vector(i*pas, j*pas, l*pas));
                ++indice;
            }
        }
    }
    return univers;
}

int main() {
    // ---- Question 6 : l'univers demande, (2^5)^3 particules ----
    const unsigned cote = 1u << 5; // 2^5 = 32 particules par axe
    Univers univers = genererCube(5);
    univers.initialiserCellules();

    std::printf("Question 6 : univers genere, %d particules sur une grille %u x %u x %u\n",
                univers.getNombreParticules(), cote, cote, cote);
    std::printf("Pas entre particules : %.6f\n", 1.0/(cote - 1));
    std::printf("Grille de cellules : %d x %d x %d\n",
                univers.getNombreCellules(0), univers.getNombreCellules(1),
                univers.getNombreCellules(2));

    std::size_t occupees = 0, particulesPlacees = 0;
    for (const auto& cellule : univers.getCellules()) {
        if (cellule.taille() > 0) ++occupees;
        particulesPlacees += cellule.taille();
    }
    std::printf("Cellules occupees : %zu / %zu, particules placees : %zu\n",
                occupees, univers.getCellules().size(), particulesPlacees);

    // ---- Question 7 : performance de la construction ----
    std::printf("\nQuestion 7 : insertion des particules dans le conteneur\n");
    std::printf("   k        n     insertion\n");
    for (unsigned k = 3; k <= 7; ++k) {
        const unsigned n = (1u << k)*(1u << k)*(1u << k);
        auto debut = std::chrono::steady_clock::now();
        Univers u = genererCube(k);
        double t = secondesDepuis(debut);
        std::printf("%4u %8u  %9.4f s\n", k, n, t);
    }

    // ---- Question 8 : performance du calcul des interactions ----
    // Le cout est en O(n^2) : on mesure jusqu'a k = 5 puis on extrapole, les
    // tailles superieures demandant des heures de calcul.
    std::printf("\nQuestion 8 : calcul des interactions (recalculerForces, O(n^2))\n");
    std::printf("   k        n        temps        paires\n");
    unsigned nReference = 0;
    double tReference = 0.0;
    for (unsigned k = 3; k <= 5; ++k) {
        const unsigned n = (1u << k)*(1u << k)*(1u << k);
        Univers u = genererCube(k);

        auto debut = std::chrono::steady_clock::now();
        u.recalculerForces();
        double t = secondesDepuis(debut);

        double paires = 0.5*double(n)*double(n - 1);
        std::printf("%4u %8u  %9.4f s  %10.3e\n", k, n, t, paires);
        nReference = n;
        tReference = t;
    }
    for (unsigned k = 6; k <= 7; ++k) {
        const unsigned n = (1u << k)*(1u << k)*(1u << k);
        double estimation = tReference*(double(n)*double(n - 1))
                            / (double(nReference)*double(nReference - 1));
        std::printf("%4u %8u  ~%8.1f s (extrapolation O(n^2), soit ~%.1f min)\n",
                    k, n, estimation, estimation / 60.0);
    }

    return 0;
}
