#ifndef UNIVERS_HPP
#define UNIVERS_HPP

#include "Particule.hpp"
#include "vector.hpp"
#include <vector>
#include <random>
#include <iostream>
#include <stdexcept>

// Represente l'ensemble des particules de la simulation.
// L'univers connait sa dimension (1, 2 ou 3) et possede la collection
// de particules qu'il fait evoluer dans le temps.
class Univers {
private:
    int dimension;                     // 1D, 2D ou 3D
    std::vector<Particule> particules;

    // Verifie l'auto-coherence de l'instance (dimension valide).
    void verifierDimension() const {
        if (dimension < 1 || dimension > 3) {
            throw std::invalid_argument("dimension doit valoir 1, 2 ou 3");
        }
    }

public:
    // Construit un univers vide de la dimension donnee.
    explicit Univers(int dimension) : dimension(dimension) {
        verifierDimension();
    }

    // Construit un univers a partir d'une collection de particules existante.
    Univers(int dimension, std::vector<Particule> particules)
        : dimension(dimension), particules(std::move(particules)) {
        verifierDimension();
    }

    int getDimension() const { return dimension; }
    int getNombreParticules() const { return static_cast<int>(particules.size()); }
    const std::vector<Particule>& getParticules() const { return particules; }

    void ajouterParticule(const Particule& p) { particules.push_back(p); }

    // Question 6 : genere nombreParticules particules de masse donnee,
    // uniformement distribuees sur le cube [0;1]^dimension, et toutes
    // dotees de la meme vitesse initiale (particules uniformes).
    void genererUniforme(int nombreParticules, const Vector& vitesse, double masse) {
        particules.clear();
        particules.reserve(nombreParticules);

        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_real_distribution<double> dist(0.0, 1.0);

        for (int id = 0; id < nombreParticules; ++id) {
            double x = dist(mt);
            double y = (dimension >= 2) ? dist(mt) : 0.0;
            double z = (dimension >= 3) ? dist(mt) : 0.0;

            particules.push_back(Particule(x, y, z,
                                            vitesse.getX(), vitesse.getY(), vitesse.getZ(),
                                            masse, id, 0,
                                            0.0, 0.0, 0.0));
        }
    }

    // Remet les forces a zero puis recalcule l'interaction gravitationnelle
    // de chaque paire de particules (O(N^2), une seule evaluation par paire).
    void calculerForces() {
        for (auto& p : particules) {
            p.setForce(Vector(0, 0, 0));
        }

        for (std::size_t i = 0; i < particules.size(); ++i) {
            for (std::size_t j = i + 1; j < particules.size(); ++j) {
                Vector d = particules[j].getPosition() - particules[i].getPosition();
                double r = d.norm();
                double F = particules[i].getMasse() * particules[j].getMasse() / (r * r * r);

                particules[i].setForce(particules[i].getForce() + d * F);
                particules[j].setForce(particules[j].getForce() - d * F); // Fji = -Fij
            }
        }
    }

    // Met a jour les vitesses a partir des forces sauvegardees avant le
    // deplacement et des forces courantes (schema de Stormer-Verlet).
    void mettreAJourVitesses(double dt, const std::vector<Vector>& forcesAvant) {
        for (std::size_t i = 0; i < particules.size(); ++i) {
            double m = particules[i].getMasse();
            Vector nouvelleVitesse = particules[i].getVitesse()
                + (forcesAvant[i] + particules[i].getForce()) * (0.5 * dt / m);
            particules[i].setVitesse(nouvelleVitesse);
        }
    }

    // Fait avancer l'univers d'un pas de temps dt (schema de Stormer-Verlet) :
    //   1. sauvegarde des forces courantes (F_old)
    //   2. mise a jour des positions avec F_old
    //   3. recalcul des forces apres deplacement
    //   4. mise a jour des vitesses avec F_old et les nouvelles forces
    void avancer(double dt) {
        std::vector<Vector> forcesAvant;
        forcesAvant.reserve(particules.size());
        for (const auto& p : particules) {
            forcesAvant.push_back(p.getForce());
        }

        for (std::size_t i = 0; i < particules.size(); ++i) {
            double m = particules[i].getMasse();
            Vector nouvellePosition = particules[i].getPosition()
                + particules[i].getVitesse() * dt
                + forcesAvant[i] * (0.5 * dt * dt / m);
            particules[i].setPosition(nouvellePosition);
        }

        calculerForces();
        mettreAJourVitesses(dt, forcesAvant);
    }

    // Affiche l'etat courant de l'univers : la position de chaque particule.
    void afficher(std::ostream& os = std::cout) const {
        for (const auto& p : particules) {
            os << "Particule " << p.getId() << " : " << p.getPosition() << "\n";
        }
    }
};

#endif // UNIVERS_HPP
