// Reponses aux questions 6-10 du TP sur la classe Univers :
// generation d'un univers uniforme, mesures de performance (insertion,
// calcul des interactions) et pistes d'optimisation.

#include "../include/Univers.hpp"
#include <chrono>
#include <cmath>
#include <iostream>
#include <random>
#include <vector>

using Clock = std::chrono::steady_clock;

double secondsSince(Clock::time_point debut) {
    return std::chrono::duration<double>(Clock::now() - debut).count();
}

// Genere un Univers de n particules de masse 1, vitesse nulle, avec des
// positions uniformement distribuees dans le cube unite [0,1]^3.
Univers genererUniversUniforme(unsigned n, unsigned seed = 42) {
    Univers u(3, n, 1.0, Vector(0, 0, 0));
    std::mt19937 mt(seed);
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    for (unsigned i = 0; i < n; ++i) {
        u.setPosition(i, Vector(dist(mt), dist(mt), dist(mt)));
    }
    return u;
}

int main() {
    // Question 6 : univers de (2^5)^3 particules uniformement distribuees.
    {
        unsigned n = static_cast<unsigned>(std::pow(1 << 5, 3)); // 32^3 = 32768
        Univers u = genererUniversUniforme(n);
        std::cout << "Univers cree avec " << u.getNombreParticules() << " particules dans [0,1]x[0,1]x[0,1].\n\n";
    }

    // Question 7 : performance de l'insertion dans le conteneur, pour des
    // tailles croissantes (insertion = O(n)).
    std::cout << " Question 7 : performance d'insertion \n";
    for (unsigned k = 3; k <= 7; ++k) {
        unsigned n = static_cast<unsigned>(std::pow(1 << k, 3));
        auto debut = Clock::now();
        Univers u = genererUniversUniforme(n);
        double t = secondsSince(debut);
        std::cout << "k=" << k << "  n=" << n << "\tinsertion : " << t << " s\n";
    }
    std::cout << "\n";

    // Question 8 : performance du calcul des interactions (O(n^2) paires).

    std::cout << " Question 8 : performance du calcul des interactions \n";
    unsigned nRef;
    double tRef;
    for (unsigned k = 3; k <= 5; ++k) {
        unsigned n = static_cast<unsigned>(std::pow(1 << k, 3));
        Univers u = genererUniversUniforme(n);

        auto debut = Clock::now();
        u.recalculerForces();
        double t = secondsSince(debut);

        std::cout << "k=" << k << "  n=" << n
                   << "\trecalculerForces : " << t << " s\n";

        nRef = n;
        tRef = t;
    }
    for (unsigned k = 6; k <= 7; ++k) {
        unsigned n = static_cast<unsigned>(std::pow(1 << k, 3));
        double tEstime = tRef * (double(n) * double(n - 1))
                        / (double(nRef) * double(nRef - 1));
        std::cout << "k=" << k << "  n=" << n
                   << "\ttemps estime (O(n^2)) : ~" << tEstime << " s"
                   << "  (~" << tEstime / 60.0 << " min)\n";
    }
    std::cout << "\n";

    // Question 9 : modification simple divisant le temps de calcul par 2.
    // Probablement - troisieme loi de Newton (Fji = -Fij) : ne calculer chaque force qu'une
    // seule fois en parcourant seulement les paires i < j. Deja fait.

    // Question 10 : autres simplifications possibles.

    // Ignorer les paires de particules trop distantes, dont la contribution 
    // a la force est negligeable.

    //utiliser comme type de Vector float et pas double.

    return 0;
}
