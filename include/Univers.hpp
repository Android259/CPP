#ifndef UNIVERS_HPP
#define UNIVERS_HPP

#include <vector>
#include <iostream>
#include <cmath>
#include <stdexcept>
#include "Particule.hpp"
#include "Cellule.hpp"

// Univers : une collection de particules evoluant par interaction gravitationnelle
class Univers {
private:
    int dimension;// 1, 2 ou 3
    int nombreParticules;
    std::vector<Particule> particules;

    // Contraintes physiques de l'univers (Question 2) :
    std::vector<double> longueursCaracteristiques; // L_d, une valeur par dimension (taille de la boite)
    double rayonCoupure;// r_cut : dehors le potentiel est negligeable
    double epsilon;// profondeur du potentiel de Lennard-Jones
    double sigma;// echelle de longueur du potentiel de Lennard-Jones

    // Grille tensorielle de cellules (Question 3) :
    std::vector<int> nombreCellules;  // nc_d = floor(L_d / rcoup), une valeur par dimension
    std::vector<Cellule> cellules;    // tableau linearise de taille nc_0 * nc_1 * ... (1 par dimension absente)

    // Calcule l'indice de cellule (coordonnee entiere) sur l'axe d pour la
    // composante donnee de la position, serre dans [0, nombreCellules[d]-1].
    int coordonneeCellule(double composante, int d) const {
        double tailleCellule = longueursCaracteristiques[d] / nombreCellules[d];
        int ic = static_cast<int>(std::floor(composante / tailleCellule));
        if (ic < 0) ic = 0;
        if (ic >= nombreCellules[d]) ic = nombreCellules[d] - 1;
        return ic;
    }

    // Coordonnees entieres (une par dimension) de la cellule contenant la
    // position donnee.
    std::vector<int> coordonneesCellule(const Vector& position) const {
        double composantes[3] = {position.getX(), position.getY(), position.getZ()};
        std::vector<int> coord(dimension);
        for (int d = 0; d < dimension; ++d) {
            coord[d] = coordonneeCellule(composantes[d], d);
        }
        return coord;
    }

    // Indice lineaire (dans 'cellules') correspondant a des coordonnees
    // entieres de cellule, quel que soit dimension (1, 2 ou 3).
    std::size_t indiceLineaire(const std::vector<int>& coord) const {
        std::size_t indice = 0;
        std::size_t multiplicateur = 1;
        for (int d = 0; d < dimension; ++d) {
            indice += static_cast<std::size_t>(coord[d]) * multiplicateur;
            multiplicateur *= static_cast<std::size_t>(nombreCellules[d]);
        }
        return indice;
    }

    // Inverse de indiceLineaire : coordonnees entieres de la cellule portant
    // l'indice lineaire donne.
    std::vector<int> coordonneesDepuisIndice(std::size_t indice) const {
        std::vector<int> coord(dimension);
        for (int d = 0; d < dimension; ++d) {
            coord[d] = static_cast<int>(indice % static_cast<std::size_t>(nombreCellules[d]));
            indice /= static_cast<std::size_t>(nombreCellules[d]);
        }
        return coord;
    }

    // Indices lineaires des cellules voisines (elle incluse) d'une cellule
    // donnee par ses coordonnees entieres : meme parcours (decalage -1,0,1
    // sur chaque dimension active, sans periodicite) que cellulesVoisines().
    std::vector<std::size_t> cellulesVoisinesDeCoord(const std::vector<int>& coord) const {
        std::vector<std::size_t> voisins;
        std::vector<int> decalage(static_cast<std::size_t>(dimension), -1);

        while (true) {
            std::vector<int> coordVoisin(dimension);
            bool valide = true;
            for (int d = 0; d < dimension; ++d) {
                coordVoisin[d] = coord[d] + decalage[d];
                if (coordVoisin[d] < 0 || coordVoisin[d] >= nombreCellules[d]) {
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

    // Calcule la force gravitationnelle entre i et j et l'accumule dans
    // force[i] et force[j] (Fji = -Fij).
    void ajouterForceGravitationnelle(std::size_t i, std::size_t j) {
        Vector d = particules[j].getPosition() - particules[i].getPosition();
        double r2 = d*d; // produit scalaire d*d = x^2+y^2+z^2 = r^2, sans recalcul
        double r = std::sqrt(r2);
        double Fij = particules[i].getMasse() * particules[j].getMasse() / (r2 * r);

        particules[i].setForce(particules[i].getForce() + d*Fij);
        particules[j].setForce(particules[j].getForce() - d*Fij);
    }

    // Calcule la force de Lennard-Jones entre i et j et l'accumule dans
    // force[i] et force[j], seulement si r <= rayonCoupure (sinon le
    // potentiel est neglige, cf. Question 1). F(r) = 24*epsilon/r^2 *
    // (2*(sigma/r)^12 - (sigma/r)^6) ; positif = repulsif, oriente le long
    // de d = position[j] - position[i].
    void ajouterForceLennardJones(std::size_t i, std::size_t j) {
        Vector d = particules[j].getPosition() - particules[i].getPosition();
        double r2 = d * d;
        if (r2 > rayonCoupure * rayonCoupure || r2 == 0.0) {
            return;
        }
        double sr2 = (sigma * sigma) / r2;
        double sr6 = sr2 * sr2 * sr2;
        double sr12 = sr6 * sr6;
        double intensite = 24.0 * epsilon / r2 * (2.0 * sr12 - sr6); // = F(r)/r

        particules[i].setForce(particules[i].getForce() - d * intensite);
        particules[j].setForce(particules[j].getForce() + d * intensite);
    }

public:
    // Construit un univers de n particules uniformes (meme masse, meme
    // vitesse initiale.

    // Conserve pour compatibilite avec les sessions precedentes (Lab3) :
    // delegue vers le constructeur complet avec des contraintes par defaut
    // (boite de cote 1 dans chaque dimension, rcoup = epsilon = sigma = 1).
    Univers(int dimension, int n, double masse, const Vector& vitesseInitiale):
                Univers(dimension, n, masse, vitesseInitiale,
                std::vector<double>(static_cast<std::size_t>(dimension), 1.0),
                1.0, 1.0, 1.0) {}

    // Constructeur complet (Question 2 lab 4) : ajoute les contraintes physiques de
    // l'univers dimension, longueurs caracteristiques par dimension (L),
    // rayon de coupure (rcoup) et constantes du potentiel de Lennard-Jones
    // (epsilon, sigma). Fonctionne pour dimension = 1, 2 ou 3 : L doit alors
    // contenir exactement 'dimension' valeurs (une par axe utilise).
    Univers(int dimension, int n, double masse, const Vector& vitesseInitiale,
            const std::vector<double>& L, double rcoup, double epsilon, double sigma):
            dimension(dimension), nombreParticules(n),
            longueursCaracteristiques(L), rayonCoupure(rcoup),
            epsilon(epsilon), sigma(sigma) {
        if (dimension < 1 || dimension > 3) {
            throw std::invalid_argument("Univers : dimension doit valoir 1, 2 ou 3");
        }
        if (static_cast<int>(L.size()) != dimension) {
            throw std::invalid_argument(
                "Univers : L doit contenir exactement 'dimension' longueurs caracteristiques");
        }
        if (rcoup <= 0.0) {
            throw std::invalid_argument("Univers : rcoup doit etre strictement positif");
        }

        particules.reserve(n);
        for (int i = 0; i < n; ++i) {
            Particule p(masse);
            p.setVitesse(vitesseInitiale);
            p.setId(i);
            particules.push_back(p);
        }
    }

    int getDimension() const { return dimension; }
    int getNombreParticules() const { return nombreParticules; }
    const std::vector<Particule>& getParticules() const { return particules; }
    Particule& getParticule(std::size_t i) { return particules[i]; }

    // Longueur caracteristique (taille de la boite) le long de l'axe d
    // (0 = x, 1 = y, 2 = z). d doit etre < dimension.
    double getLongueurCaracteristique(std::size_t d) const { return longueursCaracteristiques[d]; }
    const std::vector<double>& getLongueursCaracteristiques() const { return longueursCaracteristiques; }

    double getRayonCoupure() const { return rayonCoupure; }
    double getEpsilon() const { return epsilon; }
    double getSigma() const { return sigma; }

    // --- Grille de cellules (Question 3) ---

    // Construit la grille tensorielle (nc_d = floor(L_d / rcoup) cellules par
    // dimension) et y place les particules selon leur position courante.
    // A appeler une fois les positions initiales fixees (via setPosition).
    void initialiserCellules() {
        nombreCellules.assign(static_cast<std::size_t>(dimension), 0);
        std::size_t nombreTotalCellules = 1;
        for (int d = 0; d < dimension; ++d) {
            int nc = static_cast<int>(std::floor(longueursCaracteristiques[d] / rayonCoupure));
            if (nc < 1) nc = 1; // au moins une cellule, meme si la boite est plus petite que rcoup
            nombreCellules[d] = nc;
            nombreTotalCellules *= static_cast<std::size_t>(nc);
        }
        cellules.assign(nombreTotalCellules, Cellule());
        mettreAJourCellules();
    }

    // Revide les cellules puis replace chaque particule dans la cellule
    // correspondant a sa position courante (Question 5 : a rappeler apres
    // chaque pas de temps).
    void mettreAJourCellules() {
        for (auto& c : cellules) {
            c.vider();
        }
        for (std::size_t i = 0; i < particules.size(); ++i) {
            std::vector<int> coord = coordonneesCellule(particules[i].getPosition());
            cellules[indiceLineaire(coord)].ajouterParticule(i);
        }
    }

    int getNombreCellules(std::size_t d) const { return nombreCellules[d]; }
    const std::vector<Cellule>& getCellules() const { return cellules; }
    const Cellule& getCellule(std::size_t indiceLineaireCellule) const { return cellules[indiceLineaireCellule]; }

    // Indices lineaires (dans getCellules()) des cellules voisines de la
    // cellule contenant 'position' (elle incluse) : decalage de -1, 0, +1
    // sur chaque dimension active, sans periodicite (3 voisins en 1D,
    // 9 en 2D, 27 en 3D pour une cellule interieure ; moins en bordure).
    std::vector<std::size_t> cellulesVoisines(const Vector& position) const {
        return cellulesVoisinesDeCoord(coordonneesCellule(position));
    }

    // Permet de placer la particule i (positions initiales distinctes).
    void setPosition(std::size_t i, const Vector& position) {
        particules[i].setPosition(position);
    }

    // Remet les forces a zero puis recalcule les interactions entre toutes
    // les paires de particules.
    void recalculerForces() {
        for (auto& p : particules) {
            p.setForce(Vector(0, 0, 0));
        }
        for (std::size_t i = 0; i < particules.size(); ++i) {
            for (std::size_t j = i + 1; j < particules.size(); ++j) {
                ajouterForceGravitationnelle(i, j);
            }
        }
    }

    // Remet les forces a zero puis recalcule les interactions de
    // Lennard-Jones en ne considerant que les paires de particules dans des
    // cellules voisines (Question 4). Suppose que initialiserCellules() a
    // deja ete appele et que les cellules sont a jour (cf. mettreAJourCellules).
    void recalculerForcesLennardJones() {
        for (auto& p : particules) {
            p.setForce(Vector(0, 0, 0));
        }

        for (std::size_t iCell = 0; iCell < cellules.size(); ++iCell) {
            std::vector<int> coord = coordonneesDepuisIndice(iCell);
            std::vector<std::size_t> voisins = cellulesVoisinesDeCoord(coord);
            const auto& indicesI = cellules[iCell].getIndicesParticules();

            for (std::size_t jCell : voisins) {
                if (jCell < iCell) {
                    continue; // chaque paire de cellules traitee une seule fois
                }
                if (jCell == iCell) {
                    // paires internes a la cellule : a < b pour ne pas compter deux fois
                    for (std::size_t a = 0; a < indicesI.size(); ++a) {
                        for (std::size_t b = a + 1; b < indicesI.size(); ++b) {
                            ajouterForceLennardJones(indicesI[a], indicesI[b]);
                        }
                    }
                } else {
                    const auto& indicesJ = cellules[jCell].getIndicesParticules();
                    for (std::size_t a : indicesI) {
                        for (std::size_t b : indicesJ) {
                            ajouterForceLennardJones(a, b);
                        }
                    }
                }
            }
        }
    }

    // Fait avancer l'univers d'un pas de temps dt (schema de Stormer-Verlet).
    void avancer(double dt) {
        std::vector<Vector> forceAncienne(particules.size());
        for (std::size_t i = 0; i < particules.size(); ++i) {
            forceAncienne[i] = particules[i].getForce();
        }

        // Positions : x(t+dt) = x(t) + dt*v(t) + dt^2/(2m) * F(t)
        double demiDt2 = 0.5*dt*dt; // ne depend pas de i, calcule une seule fois
        for (std::size_t i = 0; i < particules.size(); ++i) {
            double invM = particules[i].getInvMasse();
            Vector nouvellePosition = particules[i].getPosition()
                                    + dt * particules[i].getVitesse()
                                    + (demiDt2*invM) * forceAncienne[i];
            particules[i].setPosition(nouvellePosition);
        }

        recalculerForces();

        // Vitesses : v(t+dt) = v(t) + dt/(2m) * (F(t) + F(t+dt))
        double demiDt = 0.5*dt; // ne depend pas de i, calcule une seule fois
        for (std::size_t i = 0; i < particules.size(); ++i) {
            double invM = particules[i].getInvMasse();
            Vector nouvelleVitesse = particules[i].getVitesse()
                                    + (demiDt * invM) * (forceAncienne[i] 
                                    + particules[i].getForce());
            particules[i].setVitesse(nouvelleVitesse);
        }
    }

    // Fait avancer l'univers d'un pas de temps dt avec le potentiel de
    // Lennard-Jones (schema de Stormer-Verlet), en utilisant la grille de
    // cellules pour restreindre le calcul de force aux paires voisines
    // (Question 4) et en remettant les particules a jour dans les cellules
    // a chaque pas (Question 5). A appeler apres initialiserCellules().
    void avancerLennardJones(double dt) {
        std::vector<Vector> forceAncienne(particules.size());
        for (std::size_t i = 0; i < particules.size(); ++i) {
            forceAncienne[i] = particules[i].getForce();
        }

        double demiDt2 = 0.5*dt*dt;
        for (std::size_t i = 0; i < particules.size(); ++i) {
            double invM = particules[i].getInvMasse();
            Vector nouvellePosition = particules[i].getPosition()
                                    + dt * particules[i].getVitesse()
                                    + (demiDt2*invM) * forceAncienne[i];
            particules[i].setPosition(nouvellePosition);
        }

        mettreAJourCellules(); // Question 5 : repositionner les particules dans les cellules
        recalculerForcesLennardJones();

        double demiDt = 0.5*dt;
        for (std::size_t i = 0; i < particules.size(); ++i) {
            double invM = particules[i].getInvMasse();
            Vector nouvelleVitesse = particules[i].getVitesse()
                                    + (demiDt * invM) * (forceAncienne[i]
                                    + particules[i].getForce());
            particules[i].setVitesse(nouvelleVitesse);
        }
    }

    // Affiche la position de chaque particule, reutilise operateur de vector
    friend std::ostream& operator<<(std::ostream& os, const Univers& u) {
        for (const auto& p : u.particules) {
            os << " " << p.getPosition();
        }
        return os;
    }
};

#endif // UNIVERS_HPP
