// Lab 2, Q4-Q6 : Soleil, Terre, Jupiter et Halley integres par Stormer-Verlet,
// trajectoires sauvegardees dans un fichier texte. 
// Unites : G = 1, masse du Soleil = 1, distance Terre-Soleil = 1, 
// dt = 0.015, t_end = 468.5.

// Q5 : la periode de la Terre doit valoir 2*pi, car son orbite a un rayon et
// une vitesse de 1 dans les unites de l'enonce.

#include "../include/Univers.hpp"
#include <cmath>
#include <cstdio>
#include <fstream>
#include <string>
#include <vector>

// Question 6 : ecrit une ligne par pas de temps, avec la position de chaque
// astre, sous la forme "t=0.015  Terre = <-0.015, 0.999888, 0.000000>".
// Les coordonnees passent par les accesseurs de Particule, comme demande.
void ecrireLigne(std::ofstream& fichier, double t,
                 const std::vector<Particule>& particules,
                 const std::vector<std::string>& noms) {
    fichier << "t=" << t;
    for (std::size_t i = 0; i < particules.size(); ++i) {
        const Particule& p = particules[i];
        fichier << "  " << noms[i] << " = <"
                << p.getX() << ", " << p.getY() << ", " << p.getZ() << ">";
    }
    fichier << "\n";
}

int main() {
    const double dt = 0.015;
    const double tFinal = 468.5;

    // Quatre corps, masses et etats initiaux donnes par l'enonce.
    const int nombreCorps = 4;
    const double masses[nombreCorps] = {1.0, 3.0e-6, 9.55e-4, 1.0e-14};
    const Vector positions[nombreCorps] = {
        Vector(0.0, 0.0, 0.0),// Soleil
        Vector(0.0, 1.0, 0.0),// Terre
        Vector(0.0, 5.36, 0.0),// Jupiter
        Vector(34.75, 0.0, 0.0)// Halley
    };
    const Vector vitesses[nombreCorps] = {
        Vector(0.0, 0.0, 0.0),// Soleil
        Vector(-1.0, 0.0, 0.0),// Terre
        Vector(-0.425, 0.0, 0.0),// Jupiter
        Vector(0.0, 0.0296, 0.0)// Halley
    };

    // Univers cree uniforme, puis chaque corps recoit ses propres valeurs.
    Univers univers(2, nombreCorps, 1.0, Vector(0, 0, 0));
    for (int i = 0; i < nombreCorps; ++i) {
        univers.setPosition(static_cast<std::size_t>(i), positions[i]);
        univers.getParticule(static_cast<std::size_t>(i)).setMasse(masses[i]);
        univers.getParticule(static_cast<std::size_t>(i)).setVitesse(vitesses[i]);
    }

    const std::vector<std::string> noms = {"Soleil", "Terre", "Jupiter", "Halley"};
    std::ofstream fichier("systeme_solaire.txt");
    fichier << std::fixed;  // pas de notation scientifique
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
