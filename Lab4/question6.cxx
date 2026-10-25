// Question 6 : collision carre/rectangle avec le potentiel de Lennard-Jones.
// Parametres de l'enonce (L, epsilon, sigma, N1, N2, rcut, dt). La position
// de depart des deux objets n'est pas donnee, donc centres avec un espace
// entre eux.

#include "Univers.hpp"
#include <iostream>
#include <cmath>

// Place un pave de nx*ny particules espacees de 'pas', a partir de l'indice
// indiceDepart, coin bas-gauche (xMin, yMin), meme vitesse pour toutes.
void placerGrille(Univers& univers, std::size_t indiceDepart,
                   unsigned nx, unsigned ny, double pas,
                   double xMin, double yMin, const Vector& vitesse) {
    for (unsigned j = 0; j < ny; ++j) {
        for (unsigned i = 0; i < nx; ++i) {
            std::size_t indice = indiceDepart + static_cast<std::size_t>(j * nx + i);
            univers.setPosition(indice, Vector(xMin + i * pas, yMin + j * pas, 0.0));
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
    const unsigned N1x = 40, N1y = 40;   // carre : 1600 particules
    const unsigned N2x = 160, N2y = 40;  // rectangle : 6400 particules
    const double rcoup = 2.5 * sigma;
    const double dt = 0.00005;
    const double tFinal = 19.5;
    const double pas = std::pow(2.0, 1.0 / 6.0) * sigma; // distance entre particules

    const unsigned N1 = N1x * N1y;
    const unsigned N2 = N2x * N2y;
    const unsigned N = N1 + N2;

    Univers univers(2, static_cast<int>(N), masse, Vector(0, 0, 0),
                     std::vector<double>{L1, L2}, rcoup, epsilon, sigma);

    // Rectangle bleu, immobile, en bas.
    double largeurRectangle = (N2x - 1) * pas;
    double xMinRectangle = (L1 - largeurRectangle) / 2.0;
    double yMinRectangle = 60.0;
    placerGrille(univers, 0, N2x, N2y, pas, xMinRectangle, yMinRectangle, Vector(0, 0, 0));

    // Carre rouge, au-dessus, tombe vers le rectangle.
    double largeurCarre = (N1x - 1) * pas;
    double xMinCarre = (L1 - largeurCarre) / 2.0;
    double yMinCarre = yMinRectangle + N2y * pas + 20.0;
    placerGrille(univers, static_cast<std::size_t>(N2), N1x, N1y, pas, xMinCarre, yMinCarre, vitesseCarre);

    univers.initialiserCellules();
    univers.recalculerForcesLennardJones();

    std::cout << "Simulation collision : N=" << N << " particules ("
               << N1 << " carre + " << N2 << " rectangle), dt=" << dt
               << ", jusqu'a t=" << tFinal << "\n";
    std::cout << "Grille de cellules : " << univers.getNombreCellules(0)
               << " x " << univers.getNombreCellules(1) << std::endl;

    unsigned nombrePas = static_cast<unsigned>(tFinal / dt + 0.5);
    unsigned pasAffichage = nombrePas / 100; // affiche la progression tous les 1%

    for (unsigned pasCourant = 1; pasCourant <= nombrePas; ++pasCourant) {
        univers.avancerLennardJones(dt);

        if (pasAffichage > 0 && (pasCourant % pasAffichage == 0 || pasCourant == nombrePas)) {
            double t = pasCourant * dt;

            // boite englobante + vitesse moyenne, pour suivre l'evolution sans tout logguer
            double xMin = L1, xMax = 0.0, yMin = L2, yMax = 0.0;
            double vitesseMoyenne = 0.0;
            for (unsigned i = 0; i < N; ++i) {
                const Vector& p = univers.getParticule(static_cast<std::size_t>(i)).getPosition();
                if (p.getX() < xMin) xMin = p.getX();
                if (p.getX() > xMax) xMax = p.getX();
                if (p.getY() < yMin) yMin = p.getY();
                if (p.getY() > yMax) yMax = p.getY();
                vitesseMoyenne += univers.getParticule(static_cast<std::size_t>(i)).getVitesse().norm();
            }
            vitesseMoyenne /= N;

            std::cout << "t=" << t
                       << "  boite=[" << xMin << "," << xMax << "]x[" << yMin << "," << yMax << "]"
                       << "  |v|moyenne=" << vitesseMoyenne << std::endl;
        }
    }

    std::cout << "Simulation terminee.\n";
    return 0;
}
