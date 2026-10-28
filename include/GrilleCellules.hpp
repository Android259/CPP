#ifndef GRILLE_CELLULES_HPP
#define GRILLE_CELLULES_HPP

#include <vector>
#include <array>
#include <cmath>
#include <cstddef>
#include "Vector.hpp"
#include "Particule.hpp"
#include "Cellule.hpp"

// GrilleCellules : grille tensorielle qui decoupe l'espace en cellules de
// cote ~rayonCoupure, pour ne considerer que les paires de particules
// proches lors du calcul d'un potentiel a courte portee (Question 3/4 lab4).
// Classe technique (cf. Cours 05) : independante de la physique, separee de
// Univers pour que chacune garde un objectif clair (forte cohesion).
class GrilleCellules {
private:
    int dimension;
    std::vector<double> longueursCaracteristiques;
    double rayonCoupure;

    std::vector<int> nombreCellules;  // nc_d = floor(L_d / rcoup), une valeur par dimension
    std::vector<double> tailleCellule; // = longueursCaracteristiques[d] / nombreCellules[d], mis en cache (evite de le recalculer a chaque particule, a chaque pas, dans coordonneeCellule)
    std::vector<Cellule> cellules;    // tableau linearise de taille nc_0*nc_1*... (1 par dimension absente)
    std::vector<std::size_t> celluleDeParticule; // celluleDeParticule[i] = cellule actuelle de la particule i
    std::vector<std::vector<std::size_t>> voisinsDeCellule; // voisinsDeCellule[iCell] = cellules voisines d'indice >= iCell, calcule une fois (cf. constructeur)

    // Calcule l'indice de cellule (coordonnee entiere) sur l'axe d pour la
    // composante donnee de la position, serre dans [0, nombreCellules[d]-1].
    int coordonneeCellule(double composante, int d) const {
        int ic = static_cast<int>(std::floor(composante / tailleCellule[d]));
        if (ic < 0) ic = 0;
        if (ic >= nombreCellules[d]) ic = nombreCellules[d] - 1;
        return ic;
    }

    // Coordonnees entieres (une par dimension) de la cellule contenant la
    // position donnee. std::array plutot que std::vector : pas d'allocation
    // (appelee a chaque particule, a chaque pas).
    std::array<int, 3> coordonneesCellule(const Vector& position) const {
        std::array<int, 3> coord{};
        for (int d = 0; d < dimension; ++d) {
            coord[static_cast<std::size_t>(d)] = coordonneeCellule(position[d], d);
        }
        return coord;
    }

    // Indice lineaire (dans 'cellules') correspondant a des coordonnees
    // entieres de cellule, quel que soit dimension (1, 2 ou 3).
    std::size_t indiceLineaire(const std::array<int, 3>& coord) const {
        std::size_t indice = 0;
        std::size_t multiplicateur = 1;
        for (int d = 0; d < dimension; ++d) {
            indice += static_cast<std::size_t>(coord[static_cast<std::size_t>(d)])*multiplicateur;
            multiplicateur *= static_cast<std::size_t>(nombreCellules[d]);
        }
        return indice;
    }

    // Inverse de indiceLineaire : coordonnees entieres de la cellule portant
    // l'indice lineaire donne.
    std::array<int, 3> coordonneesDepuisIndice(std::size_t indice) const {
        std::array<int, 3> coord{};
        for (int d = 0; d < dimension; ++d) {
            coord[static_cast<std::size_t>(d)] = static_cast<int>(indice % static_cast<std::size_t>(nombreCellules[d]));
            indice /= static_cast<std::size_t>(nombreCellules[d]);
        }
        return coord;
    }

    // Indices lineaires des cellules voisines (elle incluse) d'une cellule
    // donnee par ses coordonnees entieres : decalage -1,0,1 sur chaque
    // dimension active, sans periodicite.
    std::vector<std::size_t> cellulesVoisinesDeCoord(const std::array<int, 3>& coord) const {
        std::vector<std::size_t> voisins;
        std::size_t maxVoisins = 1; // au plus 3^dimension (decalage -1,0,1 sur chaque axe) : evite les reallocations de push_back
        for (int d = 0; d < dimension; ++d) maxVoisins *= 3;
        voisins.reserve(maxVoisins);
        std::array<int, 3> decalage{-1, -1, -1};

        while (true) {
            std::array<int, 3> coordVoisin{};
            bool valide = true;
            for (int d = 0; d < dimension; ++d) {
                std::size_t dd = static_cast<std::size_t>(d);
                coordVoisin[dd] = coord[dd] + decalage[dd];
                if (coordVoisin[dd] < 0 || coordVoisin[dd] >= nombreCellules[d]) {
                    valide = false;
                    break;
                }
            }
            if (valide) {
                voisins.push_back(indiceLineaire(coordVoisin));
            }

            int d = 0;
            while (d < dimension) {
                ++decalage[static_cast<std::size_t>(d)];
                if (decalage[static_cast<std::size_t>(d)] <= 1) break;
                decalage[static_cast<std::size_t>(d)] = -1;
                ++d;
            }
            if (d == dimension) break;
        }

        return voisins;
    }

public:
    // Construit la grille tensorielle (nc_d = floor(L_d / rcoup) cellules
    // par dimension) et precalcule le voisinage de chaque cellule (la
    // topologie ne depend que de L/rcoup/dimension, pas des positions).
    GrilleCellules(int dimension, const std::vector<double>& longueursCaracteristiques, double rayonCoupure)
        : dimension(dimension), longueursCaracteristiques(longueursCaracteristiques), rayonCoupure(rayonCoupure) {
        nombreCellules.assign(static_cast<std::size_t>(dimension), 0);
        tailleCellule.assign(static_cast<std::size_t>(dimension), 0.0);
        std::size_t nombreTotalCellules = 1;
        for (int d = 0; d < dimension; ++d) {
            int nc = static_cast<int>(std::floor(this->longueursCaracteristiques[d] / rayonCoupure));
            if (nc < 1) nc = 1; // au moins une cellule, meme si la boite est plus petite que rcoup
            nombreCellules[d] = nc;
            tailleCellule[d] = this->longueursCaracteristiques[d] / nc;
            nombreTotalCellules *= static_cast<std::size_t>(nc);
        }
        cellules.assign(nombreTotalCellules, Cellule());

        // Chaque paire de cellules voisines ne doit etre traitee qu'une
        // fois (cf. recalculerForcesLennardJones) : on ne garde ici que les
        // voisins d'indice >= iCell, le filtre jCell < iCell n'a donc plus
        // besoin d'etre refait a chaque pas dans Univers.
        voisinsDeCellule.resize(nombreTotalCellules);
        for (std::size_t iCell = 0; iCell < nombreTotalCellules; ++iCell) {
            std::vector<std::size_t> tous = cellulesVoisinesDeCoord(coordonneesDepuisIndice(iCell));
            std::vector<std::size_t> moitie;
            moitie.reserve(tous.size());
            for (std::size_t jCell : tous) {
                if (jCell >= iCell) moitie.push_back(jCell);
            }
            voisinsDeCellule[iCell] = std::move(moitie);
        }
    }

    // Placement initial : pas de cellule precedente, chaque particule est
    // simplement ajoutee a la cellule de sa position courante. A appeler une
    // fois les positions initiales fixees.
    void placerParticules(const std::vector<Particule>& particules) {
        celluleDeParticule.assign(particules.size(), 0);

        // Reserve une capacite basee sur la densite moyenne (marge x2 pour
        // une repartition non uniforme), pour eviter les reallocations des
        // Cellule::indicesParticules au fil de la simulation.
        if (!cellules.empty()) {
            std::size_t capacite = 2*(particules.size() / cellules.size() + 1);
            for (auto& c : cellules) {
                c.reserver(capacite);
            }
        }

        for (std::size_t i = 0; i < particules.size(); ++i) {
            std::size_t indiceCellule = indiceLineaire(coordonneesCellule(particules[i].getPosition()));
            cellules[indiceCellule].ajouterParticule(i);
            celluleDeParticule[i] = indiceCellule;
        }
    }

    // Replace chaque particule dans la cellule correspondant a sa position
    // courante (a rappeler apres chaque pas de temps). Cell-list
    // incrementale : ne touche qu'aux particules qui ont effectivement
    // change de cellule depuis le dernier appel (celluleDeParticule sert de
    // cache pour le detecter en O(1) par particule).
    void mettreAJour(const std::vector<Particule>& particules) {
        for (std::size_t i = 0; i < particules.size(); ++i) {
            std::size_t nouvelleCellule = indiceLineaire(coordonneesCellule(particules[i].getPosition()));
            std::size_t ancienneCellule = celluleDeParticule[i];
            if (nouvelleCellule != ancienneCellule) {
                cellules[ancienneCellule].retirerParticule(i);
                cellules[nouvelleCellule].ajouterParticule(i);
                celluleDeParticule[i] = nouvelleCellule;
            }
        }
    }

    int getNombreCellules(std::size_t d) const { return nombreCellules[d]; }
    const std::vector<Cellule>& getCellules() const { return cellules; }
    const Cellule& getCellule(std::size_t indiceLineaireCellule) const { return cellules[indiceLineaireCellule]; }

    // Indices lineaires (dans getCellules()) des cellules voisines deja
    // precalculees de la cellule d'indice lineaire donne.
    const std::vector<std::size_t>& voisinsDe(std::size_t indiceLineaireCellule) const {
        return voisinsDeCellule[indiceLineaireCellule];
    }

    // Indices lineaires des cellules voisines (elle incluse) de la cellule
    // contenant 'position'.
    std::vector<std::size_t> cellulesVoisines(const Vector& position) const {
        return cellulesVoisinesDeCoord(coordonneesCellule(position));
    }
};

#endif // GRILLE_CELLULES_HPP
