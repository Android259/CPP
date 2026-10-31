#ifndef UNIVERS_HPP
#define UNIVERS_HPP

#include <array>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include "Particule.hpp"
#include "Cellule.hpp"
#include "GrilleCellules.hpp"
#include "ExportateurVTK.hpp"

// Univers : une collection de particules evoluant par interaction gravitationnelle
// ou de Lennard-Jones, dans un espace de dimension Dim fixee a la compilation

template <std::size_t Dim>
class Univers {
private:
    std::vector<Particule<Dim>> particules;
    std::vector<Vector<Dim>> forceAncienne; // tampon reutilise par avancer/avancerLennardJones (evite une allocation par pas)

    // Contraintes physiques de l'univers (Question 2) :
    std::array<double, Dim> longueursCaracteristiques; // L_d, une valeur par dimension (taille de la boite)
    double rayonCoupure;// dehors - le potentiel est negligeable
    double epsilon;// profondeur du potentiel de Lennard-Jones
    double sigma;// echelle de longueur du potentiel de Lennard-Jones
    double rayonCoupureCarre; // = rayonCoupure^2, mis en cache (evite de le recalculer a chaque paire dans ajouterForceLennardJones)
    double sigmaCarre; // = sigma^2, mis en cache (meme raison)

    // Grille tensorielle de cellules (Question 3), classe technique separee
    // (Cours 05 - architecture en couches / forte cohesion) :
    GrilleCellules<Dim> grille;

    // Calcule la force gravitationnelle entre i et j et l'accumule dans
    // force[i] et force[j] (Fji = -Fij).
    void ajouterForceGravitationnelle(std::size_t i, std::size_t j) {
        Vector<Dim> d = particules[j].getPosition() - particules[i].getPosition();
        double r2 = d*d; // produit scalaire d*d = r^2, sans recalcul
        double r = std::sqrt(r2);
        double Fij = particules[i].getMasse()*particules[j].getMasse() / (r2*r);

        Vector<Dim> force = d*Fij;
        particules[i].ajouterForce(force);
        particules[j].ajouterForce(-force);
    }

    // Calcule la force de Lennard-Jones entre i et j et l'accumule dans
    // force[i] et force[j], seulement si r <= rayonCoupure (sinon le
    // potentiel est neglige). F(r) = 24*epsilon/r^2 *(2*(sigma/r)^12 - (sigma/r)^6) ;
    void ajouterForceLennardJones(std::size_t i, std::size_t j) {
        Vector<Dim> d = particules[j].getPosition() - particules[i].getPosition();
        double r2 = d*d;
        if (r2 > rayonCoupureCarre || r2 == 0.0) {
            return;
        }
        double sr2 = sigmaCarre / r2;
        double sr6 = sr2*sr2*sr2;
        double sr12 = sr6*sr6;
        double intensite = 24.0*epsilon / r2*(2.0*sr12 - sr6); // = F(r)/r

        Vector<Dim> force = d*intensite;
        particules[i].ajouterForce(-force);
        particules[j].ajouterForce(force);
    }

    // Longueurs caracteristiques par defaut (boite de cote 1 dans chaque dimension).
    static std::array<double, Dim> longueursParDefaut() {
        std::array<double, Dim> L;
        L.fill(1.0);
        return L;
    }

public:
    // Construit un univers de n particules uniformes (meme masse, meme
    // vitesse initiale).
    // Conserve pour compatibilite avec les sessions precedentes (Lab3) :
    // delegue vers le constructeur complet avec des contraintes par defaut
    // (boite de cote 1 dans chaque dimension, rcoup = epsilon = sigma = 1).
    Univers(int n, double masse, const Vector<Dim>& vitesseInitiale):
                Univers(n, masse, vitesseInitiale, longueursParDefaut(), 1.0, 1.0, 1.0) {}

    // Constructeur complet (Question 2 lab 4) : ajoute les contraintes physiques de
    // l'univers : longueurs caracteristiques par dimension (L), rayon de
    // coupure (rcoup) et constantes du potentiel de Lennard-Jones (epsilon, sigma).
    Univers(int n, double masse, const Vector<Dim>& vitesseInitiale,
            const std::array<double, Dim>& L, double rcoup, double epsilon, double sigma):
            longueursCaracteristiques(L), rayonCoupure(rcoup),
            epsilon(epsilon), sigma(sigma),
            rayonCoupureCarre(rcoup*rcoup), sigmaCarre(sigma*sigma),
            grille(L, rcoup) {
        particules.reserve(n);
        for (int i = 0; i < n; ++i) {
            Particule<Dim> p(masse);
            p.setVitesse(vitesseInitiale);
            p.setId(i);
            particules.push_back(p);
        }
        forceAncienne.resize(particules.size());
    }

    // Construit un univers a partir d'une liste de particules deja prete
    // (masse/position/vitesse/id quelconques pour chacune), plutot que de
    // les construire toutes identiques a l'interieur de Univers. Separe la
    // creation des particules (systeme heterogene, ex. systeme solaire) de
    // la logique de Univers.
    Univers(const std::vector<Particule<Dim>>& listeParticules,
            const std::array<double, Dim>& L, double rcoup, double epsilon, double sigma):
            particules(listeParticules),
            longueursCaracteristiques(L), rayonCoupure(rcoup),
            epsilon(epsilon), sigma(sigma),
            rayonCoupureCarre(rcoup*rcoup), sigmaCarre(sigma*sigma),
            grille(L, rcoup) {
        forceAncienne.resize(particules.size());
    }

    static constexpr std::size_t getDimension() { return Dim; }
    int getNombreParticules() const { return static_cast<int>(particules.size()); }
    const std::vector<Particule<Dim>>& getParticules() const { return particules; }
    Particule<Dim>& getParticule(std::size_t i) { return particules[i]; }

    // Longueur caracteristique (taille de la boite) le long de l'axe d.
    // d doit etre < Dim.
    double getLongueurCaracteristique(std::size_t d) const { return longueursCaracteristiques[d]; }
    const std::array<double, Dim>& getLongueursCaracteristiques() const { return longueursCaracteristiques; }

    double getRayonCoupure() const { return rayonCoupure; }
    double getEpsilon() const { return epsilon; }
    double getSigma() const { return sigma; }

    // --- Grille de cellules (Question 3) ---

    // Place les particules dans la grille selon leur position courante
    // (la topologie de la grille est deja construite, cf. constructeur).
    // A appeler une fois les positions initiales fixees (via setPosition).
    void initialiserCellules() {
        grille.placerParticules(particules);
    }

    // Replace chaque particule dans la cellule correspondant a sa position
    // courante (Question 5 : a rappeler apres chaque pas de temps).
    void mettreAJourCellules() {
        grille.mettreAJour(particules);
    }

    int getNombreCellules(std::size_t d) const { return grille.getNombreCellules(d); }
    const std::vector<Cellule>& getCellules() const { return grille.getCellules(); }
    const Cellule& getCellule(std::size_t indiceLineaireCellule) const { return grille.getCellule(indiceLineaireCellule); }

    // Indices lineaires (dans getCellules()) des cellules voisines de la
    // cellule contenant 'position' (elle incluse) : decalage de -1, 0, +1
    // sur chaque dimension, sans periodicite (3 voisins en 1D,
    // 9 en 2D, 27 en 3D pour une cellule interieure ; moins en bordure).
    std::vector<std::size_t> cellulesVoisines(const Vector<Dim>& position) const {
        return grille.cellulesVoisines(position);
    }

    // Permet de placer la particule i (positions initiales distinctes).
    void setPosition(std::size_t i, const Vector<Dim>& position) {
        particules[i].setPosition(position);
    }

    // Remet les forces a zero puis recalcule les interactions entre toutes
    // les paires de particules.
    void recalculerForces() {
        for (auto& p : particules) {
            p.setForce(Vector<Dim>());
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
            p.setForce(Vector<Dim>());
        }
        const auto& cellules = grille.getCellules();
        for (std::size_t iCell = 0; iCell < cellules.size(); ++iCell) {
            const auto& voisins = grille.voisinsDe(iCell);
            const auto& indicesI = cellules[iCell].getIndicesParticules();

            for (std::size_t jCell : voisins) {
                // grille.voisinsDe(iCell) ne contient deja que jCell >= iCell
                // (filtre a la construction, cf. GrilleCellules).
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
        for (std::size_t i = 0; i < particules.size(); ++i) {
            forceAncienne[i] = particules[i].getForce();
        }
        // Positions : x(t+dt) = x(t) + dt*v(t) + dt^2/(2m)*F(t)
        double demiDt2 = 0.5*dt*dt; // ne depend pas de i, calcule une seule fois
        for (std::size_t i = 0; i < particules.size(); ++i) {
            double invM = particules[i].getInvMasse();
            Vector<Dim> nouvellePosition = particules[i].getPosition()
                                    + dt*particules[i].getVitesse()
                                    + (demiDt2*invM)*forceAncienne[i];
            particules[i].setPosition(nouvellePosition);
        }

        recalculerForces();

        // Vitesses : v(t+dt) = v(t) + dt/(2m)*(F(t) + F(t+dt))
        double demiDt = 0.5*dt; // ne depend pas de i, calcule une seule fois
        for (std::size_t i = 0; i < particules.size(); ++i) {
            double invM = particules[i].getInvMasse();
            Vector<Dim> nouvelleVitesse = particules[i].getVitesse()
                                    + (demiDt*invM)*(forceAncienne[i]
                                    + particules[i].getForce());
            particules[i].setVitesse(nouvelleVitesse);
        }
    }

    // Fait avancer l'univers d'un pas de temps dt avec le potentiel de Lennard-Jones,
    // en utilisant la grille de cellules pour restreindre le calcul de force aux paires
    // voisines (Question 4) et en remettant les particules a jour dans les cellules
    // a chaque pas (Question 5). A appeler apres initialiserCellules().
    void avancerLennardJones(double dt) {
        for (std::size_t i = 0; i < particules.size(); ++i) {
            forceAncienne[i] = particules[i].getForce();
        }

        double demiDt2 = 0.5*dt*dt;
        for (std::size_t i = 0; i < particules.size(); ++i) {
            double invM = particules[i].getInvMasse();
            Vector<Dim> nouvellePosition = particules[i].getPosition()
                                    + dt*particules[i].getVitesse()
                                    + (demiDt2*invM)*forceAncienne[i];
            particules[i].setPosition(nouvellePosition);
        }

        mettreAJourCellules(); // Question 5 : repositionner les particules dans les cellules
        recalculerForcesLennardJones();

        double demiDt = 0.5*dt;
        for (std::size_t i = 0; i < particules.size(); ++i) {
            double invM = particules[i].getInvMasse();
            Vector<Dim> nouvelleVitesse = particules[i].getVitesse()
                                    + (demiDt*invM)*(forceAncienne[i]
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

    // Question 3 (Visualisation) : enregistre l'etat courant (positions,
    // vitesses, masses) dans un fichier au format VTK XML UnstructuredGrid
    // (.vtu), visualisable avec Paraview ou Visit. Pour suivre une
    // simulation dans le temps, appeler cette methode a chaque pas voulu
    // avec un nom de fichier different (ex. "frame_0001.vtu").
    void sauvegarderVTK(const std::string& nomFichier) const {
        ExportateurVTK::exporter(particules, nomFichier);
    }
};

#endif // UNIVERS_HPP
