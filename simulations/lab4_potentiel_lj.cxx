// Lab 4, Q1 : le potentiel de Lennard-Jones en fonction de r, compare a la
// gravitation.
//
// En dessous de sigma, le terme en (sigma/r)^12 domine et la force est tres
// repulsive : deux particules ne peuvent pas s'interpenetrer. Le minimum
// U = -epsilon tombe en r_eq = 2^(1/6)*sigma, ou la force s'annule : c'est
// autour de cette distance que les particules s'organisent, et c'est le pas
// utilise pour les reseaux de lab4_collision. Plus loin, la force devient
// attractive puis s'efface vite, en 1/r^7.
//
// La gravitation est toujours attractive : toutes les paires
// comptent, d'ou O(n^2). 

#include <cmath>
#include <cstdio>
#include <fstream>

// Potentiel de Lennard-Jones U(r) = 4*epsilon*((sigma/r)^12 - (sigma/r)^6).
double potentielLennardJones(double r, double epsilon, double sigma) {
    double sr6 = std::pow(sigma / r, 6);
    return 4.0*epsilon*(sr6*sr6 - sr6);
}

// Force radiale derivee du potentiel : F(r) = -dU/dr.
// Positive = repulsive, negative = attractive.
double forceLennardJones(double r, double epsilon, double sigma) {
    double sr6 = std::pow(sigma / r, 6);
    double sr12 = sr6*sr6;
    return 24.0*epsilon / r*(2.0*sr12 - sr6);
}

// Force gravitationnelle entre deux masses unitaires (G = 1), pour comparaison.
// Toujours negative : la gravitation est purement attractive.
double forceGravitationnelle(double r) {
    return -1.0 / (r*r);
}

int main() {
    const double epsilon = 1.0;
    const double sigma = 1.0;

    const double rMin = 0.9*sigma;
    const double rMax = 3.0*sigma;
    const double dr = 0.01*sigma;

    // Fichier de donnees pour tracer les courbes (gnuplot, python, ...).
    std::ofstream donnees("potentiel_lj.txt");
    donnees << "# r  U_LJ  F_LJ  F_gravitation\n";
    donnees.precision(10);
    for (double r = rMin; r <= rMax + 1e-12; r += dr) {
        donnees << r << " " << potentielLennardJones(r, epsilon, sigma)
                << " " << forceLennardJones(r, epsilon, sigma)
                << " " << forceGravitationnelle(r) << "\n";
    }
    std::printf("Courbes ecrites dans potentiel_lj.txt (colonnes : r, U_LJ, F_LJ, F_grav)\n\n");

    // Apercu dans le terminal, avec un pas plus large.
    std::printf("    r      U_LJ        F_LJ       F_grav\n");
    for (double r = rMin; r <= rMax + 1e-12; r += 0.1*sigma) {
        std::printf("%6.2f  %10.4f  %10.4f  %10.4f\n",
                    r, potentielLennardJones(r, epsilon, sigma),
                    forceLennardJones(r, epsilon, sigma),
                    forceGravitationnelle(r));
    }

    // Points caracteristiques : zero du potentiel et minimum (equilibre).
    const double rEquilibre = std::pow(2.0, 1.0 / 6.0)*sigma;
    std::printf("\nU(sigma) = %.6f  (le potentiel change de signe en r = sigma)\n",
                potentielLennardJones(sigma, epsilon, sigma));
    std::printf("r_eq = 2^(1/6)*sigma = %.6f\n", rEquilibre);
    std::printf("U(r_eq) = %.6f  (minimum attendu : -epsilon = %.6f)\n",
                potentielLennardJones(rEquilibre, epsilon, sigma), -epsilon);
    std::printf("F_LJ(r_eq) = %.3e  (attendu : 0, force nulle a l'equilibre)\n",
                forceLennardJones(rEquilibre, epsilon, sigma));
    std::printf("F_LJ(2.5sig) = %.6f  contre F_grav(2.5) = %.6f\n",
                forceLennardJones(2.5*sigma, epsilon, sigma),
                forceGravitationnelle(2.5*sigma));
    std::printf("F_LJ(5sig) = %.6f  contre F_grav(5) = %.6f\n",
                forceLennardJones(5.0*sigma, epsilon, sigma),
                forceGravitationnelle(5.0*sigma));
     // Le potentiel est negatif des deux cotes, leur rapport est donc positif :
    // pas besoin de prendre une valeur absolue. Au minimum, U vaut -epsilon.
    std::printf("\nA 5 sigma, Lennard-Jones est ~%.0f fois plus faible qu'a l'equilibre,\n"
                "ce qui justifie de negliger le potentiel au-dela de rayonCoupure.\n",
                potentielLennardJones(rEquilibre, epsilon, sigma)
                / potentielLennardJones(5.0*sigma, epsilon, sigma));

    return 0;
}
