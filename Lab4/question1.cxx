// Question 1 Lab 4
// Calcule le potentiel de Lennard-Jones U(r) pour 2 particules,
// la force (radiale) qui en derive, et la force gravitationelle pour
// comparaison. Affiche les resultats sous forme de tableau dans le terminal.

#include <cmath>
#include <iostream>

// Potentiel de Lennard-Jones.
double lennardJones(double r, double epsilon, double sigma) {
    double sr6 = std::pow(sigma/r, 6);
    return 4.0*epsilon*(sr6*sr6 - sr6);
}

// Force (radiale) derivee du potentiel de Lennard-Jones : F(r) = -dU/dr.
double lennardJonesForce(double r, double epsilon, double sigma) {
    double sr6 = std::pow(sigma/r, 6);
    double sr12 = sr6*sr6;
    return 24.0 * epsilon / r * (2.0 * sr12 - sr6);
}

// Force gravitationnelle (masses unitaires, G = 1), pour comparaison.
// Toujours negative
double forceGravite(double r) {
    return -1.0/(r*r);
}

int main() {
    const double epsilon = 1.0;
    const double sigma = 1.0;

    const double rMin = 0.8 * sigma;
    const double rMax = 3.0 * sigma;
    const double dr = 0.1 * sigma;

    for (double r = rMin; r <= rMax + 1e-9; r += dr) {
        std::cout << "r = " << r
        << "  U_LJ = " << lennardJones(r, epsilon, sigma)
        << "  F_LJ = " << lennardJonesForce(r, epsilon, sigma)
        << "  F_grav = " << forceGravite(r) << "\n";
    }

    double rEq = std::pow(2.0, 1.0 / 6.0) * sigma;
    std::cout << "Distance d'equilibre r_eq = 2^(1/6) * sigma = " << rEq << "\n";
    std::cout << "U(r_eq)   = " << lennardJones(rEq, epsilon, sigma)
    << " (minimum attendu : -epsilon = " << -epsilon << ")\n";

    std::cout << "F_LJ(r_eq) = " << lennardJonesForce(rEq, epsilon, sigma)
    << " (attendu : 0, force nulle a l'equilibre)\n";

    return 0;
}
