// Lab 4, Q6 : collision de deux objets sous Lennard-Jones, avec sauvegarde VTK
// pour Paraview. Parametres de l'enonce : L = 250x250, epsilon = 5, sigma = 1,
// m = 1, v = (0,-10) pour le carre, 40x40 et 160x40 particules espacees de
// 2^(1/6)*sigma, rayonCoupure = 2.5*sigma, dt = 0.00005, jusqu'a t = 19.5.

#include "../include/Univers.hpp"
#include <cmath>
#include <cstdio>
#include <filesystem>
#include <string>

// Place un pave de nx*ny particules espacees de 'pas', a partir de l'indice
// indiceDepart, coin bas-gauche (xMin, yMin), meme vitesse pour toutes.
void placerGrille(Univers& univers, std::size_t indiceDepart,
                  unsigned nx, unsigned ny, double pas,
                  double xMin, double yMin, const Vector& vitesse) {
    for (unsigned j = 0; j < ny; ++j) {
        for (unsigned i = 0; i < nx; ++i) {
            std::size_t indice = indiceDepart + static_cast<std::size_t>(j*nx + i);
            univers.setPosition(indice, Vector(xMin + i*pas, yMin + j*pas, 0.0));
            univers.getParticule(indice).setVitesse(vitesse);
        }
    }
}

int main() {

    const double L1 = 250.0, L2 = 250.0;
    const double epsilon = 5.0;
    const double sigma = 1.0;
    const double masse = 1.0;
    const Vector vitesseCarre(0.0, -10.0, 0.0);
    const unsigned N1x = 40, N1y = 40;    // carre : 1600 particules
    const unsigned N2x = 160, N2y = 40;   // rectangle : 6400 particules
    const double rcoup = 2.5*sigma;
    const double dt = 0.00005;
    const double pas = std::pow(2.0, 1.0 / 6.0)*sigma;

    const unsigned N1 = N1x*N1y;
    const unsigned N2 = N2x*N2y;
    const unsigned N = N1 + N2;

    Univers univers(2, static_cast<int>(N), masse, Vector(0, 0, 0),
                    std::vector<double>{L1, L2}, rcoup, epsilon, sigma);

    // Rectangle immobile, en bas.
    double largeurRectangle = (N2x - 1)*pas;
    double xMinRectangle = (L1 - largeurRectangle) / 2.0;
    double yMinRectangle = 60.0;
    placerGrille(univers, 0, N2x, N2y, pas, xMinRectangle, yMinRectangle, Vector(0, 0, 0));

    // Carre au-dessus, qui tombe vers le rectangle.
    double largeurCarre = (N1x - 1)*pas;
    double xMinCarre = (L1 - largeurCarre) / 2.0;
    double yMinCarre = yMinRectangle + N2y*pas + 20.0;
    placerGrille(univers, static_cast<std::size_t>(N2), N1x, N1y, pas,
                xMinCarre, yMinCarre, vitesseCarre);

    univers.initialiserCellules();
    univers.recalculerForcesLennardJones();

    const double tFinal = 19.5;                   // valeur demandee par l'enonce
    const unsigned nombrePas = tFinal / dt;       // 390000 pas
    const unsigned pasParFrame = nombrePas / 100; // 100 fichiers VTK

    std::filesystem::create_directory("frames");

    std::printf("Collision : N = %u particules (%u carre + %u rectangle)\n", N, N1, N2);
    std::printf("dt = %g, t_final = %g, soit %u pas\n", dt, tFinal, nombrePas);
    std::printf("Grille de cellules : %d x %d\n",
                univers.getNombreCellules(0), univers.getNombreCellules(1));
    std::printf("Une frame VTK tous les %u pas, dans frames/\n\n", pasParFrame);

    unsigned numeroFrame = 0;
    char nomFichier[64];
    std::snprintf(nomFichier, sizeof(nomFichier), "frames/collision_%04u.vtu", numeroFrame++);
    univers.sauvegarderVTK(nomFichier);

    for (unsigned pasCourant = 1; pasCourant <= nombrePas; ++pasCourant) {
        univers.avancerLennardJones(dt);

        if (pasCourant % pasParFrame == 0) {
            std::snprintf(nomFichier, sizeof(nomFichier), "frames/collision_%04u.vtu", numeroFrame++);
            univers.sauvegarderVTK(nomFichier);

            // Suivi de l'evolution : boite englobante et vitesse moyenne.
            double xMin = L1, xMax = 0.0, yMin = L2, yMax = 0.0, vitesseMoyenne = 0.0;
            for (const auto& p : univers.getParticules()) {
                if (p.getX() < xMin) xMin = p.getX();
                if (p.getX() > xMax) xMax = p.getX();
                if (p.getY() < yMin) yMin = p.getY();
                if (p.getY() > yMax) yMax = p.getY();
                vitesseMoyenne += p.getVitesse().norm();
            }
            vitesseMoyenne /= N;
            std::printf("t = %7.3f  boite = [%7.2f, %7.2f] x [%7.2f, %7.2f]  |v|moy = %7.3f\n",
                        pasCourant*dt, xMin, xMax, yMin, yMax, vitesseMoyenne);
        }
    }

    std::printf("\nSimulation terminee : %u frames dans frames/ (ouvrir collision_..vtu dans Paraview).\n",
                numeroFrame);
    return 0;
}
