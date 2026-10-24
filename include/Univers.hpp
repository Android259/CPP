#ifndef UNIVERS_HPP
#define UNIVERS_HPP

#include <vector>
#include <iostream>
#include <cmath>
#include "Particule.hpp"

// Univers : une collection de particules evoluant par interaction gravitationnelle
class Univers {
private:
    int dimension;
    int nombreParticules;
    std::vector<Particule> particules;

    // Calcule la force gravitationnelle entre i et j et l'accumule dans
    // force[i] et force[j] (Fji = -Fij).
    void ajouterForceGravitationnelle(std::size_t i, std::size_t j) {
        Vector d = particules[j].getPosition() - particules[i].getPosition();
        double r2 = d * d; // produit scalaire d*d = x^2+y^2+z^2 = r^2, sans recalcul
        double r = std::sqrt(r2);
        double Fij = particules[i].getMasse() * particules[j].getMasse() / (r2 * r);

        particules[i].setForce(particules[i].getForce() + d*Fij);
        particules[j].setForce(particules[j].getForce() - d*Fij);
    }

public:
    // Construit un univers de 'n' particules uniformes (meme masse, meme
    // vitesse initiale, positionnees a l'origine).
    Univers(int dimension, int n, double masse, const Vector& vitesseInitiale)
        : dimension(dimension), nombreParticules(n) {
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

    // Fait avancer l'univers d'un pas de temps dt (schema de Stormer-Verlet).
    void avancer(double dt) {
        std::vector<Vector> forceAncienne(particules.size());
        for (std::size_t i = 0; i < particules.size(); ++i) {
            forceAncienne[i] = particules[i].getForce();
        }

        // Positions : x(t+dt) = x(t) + dt*v(t) + dt^2/(2m) * F(t)
        double demiDt2 = 0.5 * dt * dt; // ne depend pas de i, calcule une seule fois
        for (std::size_t i = 0; i < particules.size(); ++i) {
            double invM = particules[i].getInvMasse();
            Vector nouvellePosition = particules[i].getPosition()
                + dt * particules[i].getVitesse()
                + (demiDt2 * invM) * forceAncienne[i];
            particules[i].setPosition(nouvellePosition);
        }

        recalculerForces();

        // Vitesses : v(t+dt) = v(t) + dt/(2m) * (F(t) + F(t+dt))
        double demiDt = 0.5 * dt; // ne depend pas de i, calcule une seule fois
        for (std::size_t i = 0; i < particules.size(); ++i) {
            double invM = particules[i].getInvMasse();
            Vector nouvelleVitesse = particules[i].getVitesse()
                + (demiDt * invM) * (forceAncienne[i] + particules[i].getForce());
            particules[i].setVitesse(nouvelleVitesse);
        }
    }

    // Affiche la position de chaque particule, en reutilisant l'operateur
    // d'affichage deja defini sur Vector.
    friend std::ostream& operator<<(std::ostream& os, const Univers& u) {
        for (const auto& p : u.particules) {
            os << " " << p.getPosition();
        }
        return os;
    }
};

#endif // UNIVERS_HPP
