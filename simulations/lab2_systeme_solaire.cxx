// Lab 2, Q4-Q6 : Soleil, Terre, Jupiter et Halley integres par Stormer-Verlet,
// trajectoires sauvegardees dans un fichier texte.
// Unites : G = 1, masse du Soleil = 1, distance Terre-Soleil = 1,
// dt = 0.015, t_end = 468.5.
//
// Orbites planaires : Dim = 2 (toutes les positions/vitesses de l'enonce ont
// une composante z nulle, l'univers est donc reellement a 2 dimensions).

// Q5 : la periode de la Terre doit valoir 2*pi, car son orbite a un rayon et
// une vitesse de 1 dans les unites de l'enonce.

#include "../include/Univers.hpp"
#include <cmath>
#include <cstdio>
#include <fstream>
#include <string>
#include <vector>

// Question 6 : ecrit une ligne par pas de temps, avec la position de chaque
// astre, sous la forme "t=0.015  Terre = <-0.015, 0.999888>".
// Les coordonnees passent par les accesseurs de Particule, comme demande.
void ecrireLigne(std::ofstream& fichier, double t,
                 const std::vector<Particule<2>>& particules,
                 const std::vector<std::string>& noms) {
    fichier << "t=" << t;
    for (std::size_t i = 0; i < particules.size(); ++i) {
        const Particule<2>& p = particules[i];
        fichier << "  " << noms[i] << " = <"
                << p.getX() << ", " << p.getY() << ">";
    }
    fichier << "\n";
}

int main() {
    const double dt = 0.015;
    const double tFinal = 468.5;

    // Quatre corps, masses et etats initiaux donnes par l'enonce. Chaque
    // particule est construite directement avec ses propres valeurs (masse,
    // position, vitesse) : pas de corps "generique" a corriger ensuite.
    std::vector<Particule<2>> corps = {
        Particule<2>(1.0, Vector<2>({0.0, 0.0}), Vector<2>({0.0, 0.0}), Vector<2>(), 0, 0), // Soleil
        Particule<2>(3.0e-6, Vector<2>({0.0, 1.0}), Vector<2>({-1.0, 0.0}), Vector<2>(), 1, 0),  // Terre
        Particule<2>(9.55e-4, Vector<2>({0.0, 5.36}), Vector<2>({-0.425, 0.0}), Vector<2>(), 2, 0), // Jupiter
        Particule<2>(1.0e-14, Vector<2>({34.75, 0.0}), Vector<2>({0.0, 0.0296}), Vector<2>(), 3, 0) // Halley
    };

    Univers<2> univers(corps, std::array<double, 2>{1.0, 1.0}, 1.0, 1.0, 1.0);

    const std::vector<std::string> noms = {"Soleil", "Terre", "Jupiter", "Halley"};
    std::ofstream fichier("systeme_solaire.txt");
    fichier << std::fixed;
    fichier.precision(6);

    univers.recalculerForces(); // forces initiales, avant la boucle en temps

    const unsigned nombrePas = static_cast<unsigned>(tFinal / dt);
    // Detection de la periode de la Terre : on compte les passages de son
    // ordonnee de negatif a positif (un passage par tour).
    double yPrecedent = univers.getParticule(1).getPosition().getY();
    unsigned nombreTours = 0;
    double tPremierTour = -1.0, tDernierTour = -1.0;

    std::printf("Simulation : %u pas de dt = %g jusqu'a t = %g\n", nombrePas, dt, tFinal);
    ecrireLigne(fichier, 0.0, univers.getParticules(), noms);

    for (unsigned pas = 1; pas <= nombrePas; ++pas) {
        univers.avancer(dt);
        double t = pas*dt;
        ecrireLigne(fichier, t, univers.getParticules(), noms);

        double y = univers.getParticule(1).getPosition().getY();
        if (yPrecedent < 0.0 && y >= 0.0) {
            ++nombreTours;
            if (tPremierTour < 0.0) {
                tPremierTour = t;
            }
            tDernierTour = t;
        }
        yPrecedent = y;
    }

    std::printf("Trajectoires ecrites dans systeme_solaire.txt (%u lignes).\n\n", nombrePas + 1);

    std::printf("Verification - periode orbitale de la Terre :\n");
    if (nombreTours >= 2) {
        double periode = (tDernierTour - tPremierTour) / (nombreTours - 1);
        std::printf("  %u tours effectues, periode mesuree = %.6f\n", nombreTours, periode);
        std::printf("  periode attendue 2*pi = %.6f, ecart relatif = %.3e\n",
                    2.0*M_PI, std::fabs(periode - 2.0*M_PI) / (2.0*M_PI));
    }

    return 0;
}
