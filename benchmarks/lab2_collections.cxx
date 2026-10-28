// Lab 2, Q2-Q3 : une collection de particules tirees au hasard, et la
// comparaison de vector, deque, list et set. On chronometre l'insertion et le
// parcours complet, repete pour que la difference de localite memoire ressorte.
//
// Q3 : a l'insertion deque gagne, car vector doit reallouer en grandissant.
// Au parcours vector gagne, grace a sa memoire contigue ; list et set sautent
// de noeud en noeud et sont 3 a 7 fois plus lents.
// L'ecart devient net vers 1000 particules.
// Pour la simulation, qui parcourt tout a chaque pas, vector convient.
// A compiler avec -O2 : sinon le cout des appels non inlines masque les ecarts.

#include "../include/Particule.hpp"
#include <chrono>
#include <cstdio>
#include <deque>
#include <list>
#include <random>
#include <set>
#include <vector>


double secondesDepuis(std::chrono::steady_clock::time_point debut) {
    return std::chrono::duration<double>(std::chrono::steady_clock::now() - debut).count();
}

// Tire une particule aleatoire (masse, position et vitesse dans [0,1]).
Particule particuleAleatoire(std::mt19937& mt, std::uniform_real_distribution<double>& dist, int id) {
    double masse = 0.5 + dist(mt); // masse dans [0.5, 1.5], jamais nulle
    Particule p(masse);
    p.setPosition(Vector(dist(mt), dist(mt), dist(mt)));
    p.setVitesse(Vector(dist(mt), dist(mt), dist(mt)));
    p.setId(id);
    return p;
}

// Tire les n particules une fois pour toutes. Les quatre conteneurs recoivent
// les memes, et le tirage reste hors du chronometre : la colonne "insertion"
// ne mesure alors que l'insertion, pas la generation aleatoire.
std::vector<Particule> tirerParticules(unsigned n) {
    std::mt19937 mt(42);
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    std::vector<Particule> source;
    source.reserve(n);
    for (unsigned i = 0; i < n; ++i) {
        source.push_back(particuleAleatoire(mt, dist, static_cast<int>(i)));
    }
    return source;
}

// Mesure l'insertion puis le parcours repete, pour chacun des 4 conteneurs.
// std::set exige un ordre sur ses elements : il utilise l'operator< de Particule.

void mesurerVector(const std::vector<Particule>& source, unsigned repetitionsParcours) {
    std::vector<Particule> conteneur;
    auto debut = std::chrono::steady_clock::now();
    for (const auto& p : source) {
        conteneur.push_back(p);
    }
    double tInsertion = secondesDepuis(debut);

    debut = std::chrono::steady_clock::now();
    double somme = 0.0;
    for (unsigned r = 0; r < repetitionsParcours; ++r) {
        for (const auto& p : conteneur) {
            somme += p.getMasse();
        }
    }
    double tParcours = secondesDepuis(debut);

    std::printf("  %-8s insertion %9.3f us   parcours %9.3f us   (somme = %.1f)\n",
                "vector", tInsertion*1e6, tParcours*1e6, somme);
}

void mesurerDeque(const std::vector<Particule>& source, unsigned repetitionsParcours) {
    std::deque<Particule> conteneur;
    auto debut = std::chrono::steady_clock::now();
    for (const auto& p : source) {
        conteneur.push_back(p);
    }
    double tInsertion = secondesDepuis(debut);

    debut = std::chrono::steady_clock::now();
    double somme = 0.0;
    for (unsigned r = 0; r < repetitionsParcours; ++r) {
        for (const auto& p : conteneur) {
            somme += p.getMasse();
        }
    }
    double tParcours = secondesDepuis(debut);

    std::printf("  %-8s insertion %9.3f us   parcours %9.3f us   (somme = %.1f)\n",
                "deque", tInsertion*1e6, tParcours*1e6, somme);
}

void mesurerList(const std::vector<Particule>& source, unsigned repetitionsParcours) {
    std::list<Particule> conteneur;
    auto debut = std::chrono::steady_clock::now();
    for (const auto& p : source) {
        conteneur.push_back(p);
    }
    double tInsertion = secondesDepuis(debut);

    debut = std::chrono::steady_clock::now();
    double somme = 0.0;
    for (unsigned r = 0; r < repetitionsParcours; ++r) {
        for (const auto& p : conteneur) {
            somme += p.getMasse();
        }
    }
    double tParcours = secondesDepuis(debut);

    std::printf("  %-8s insertion %9.3f us   parcours %9.3f us   (somme = %.1f)\n",
                "list", tInsertion*1e6, tParcours*1e6, somme);
}

void mesurerSet(const std::vector<Particule>& source, unsigned repetitionsParcours) {
    std::set<Particule> conteneur;
    auto debut = std::chrono::steady_clock::now();
    for (const auto& p : source) {
        conteneur.insert(p);
    }
    double tInsertion = secondesDepuis(debut);

    debut = std::chrono::steady_clock::now();
    double somme = 0.0;
    for (unsigned r = 0; r < repetitionsParcours; ++r) {
        for (const auto& p : conteneur) {
            somme += p.getMasse();
        }
    }
    double tParcours = secondesDepuis(debut);

    std::printf("  %-8s insertion %9.3f us   parcours %9.3f us   (somme = %.1f)\n",
                "set", tInsertion*1e6, tParcours*1e6, somme);
}

int main() {
    std::printf("Comparaison des conteneurs (parcours repete 1000 fois)\n");

    for (unsigned n = 64; n <= 8192; n *= 2) {
        std::printf("\nn = %u particules\n", n);
        const unsigned repetitions = 1000;
        const std::vector<Particule> source = tirerParticules(n);

        mesurerVector(source, repetitions);
        mesurerDeque(source, repetitions);
        mesurerList(source, repetitions);
        mesurerSet(source, repetitions);
    }
    return 0;
}
