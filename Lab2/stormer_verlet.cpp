#include "Particule.hpp"
#include <vector>
#include <random>
#include <chrono>
#include <iostream>

double gravitational_force_ij(const std::vector<Particule>& particules_vector, int i, int j, const Vector& d) {
    double r = d.norm();
    return particules_vector[i].getMasse() * particules_vector[j].getMasse()
                                        / (r*r*r);
}

double gravitational_force_ij(const std::vector<Particule>& particules_vector, int i, int j) {
    Vector d = particules_vector[j].getPosition() - particules_vector[i].getPosition();
    return gravitational_force_ij(particules_vector, i, j, d);
}

// Accumule dans force[i] et force[j] les contributions des paires (i, j>i).
// Attention : ADDITIVE, il faut remettre force a zero avant d'appeler cette
// fonction pour i = 0..N-1 a chaque pas de temps (voir recalculer_forces).
void renew_gravitational_force_dxdydz_i(std::vector<Particule>& particules_vector, int i) {
    for (std::size_t j = i + 1; j < particules_vector.size(); ++j) {

        Vector d = particules_vector[j].getPosition() - particules_vector[i].getPosition();

        double F_ij = gravitational_force_ij(particules_vector, i, j, d);

        // Fij
        particules_vector[i].setForce(particules_vector[i].getForce() + d * F_ij);

        // Fji = -Fij
        particules_vector[j].setForce(particules_vector[j].getForce() - d * F_ij);
    }
}

void renew_positions_Stormer_Verlet(std::vector<Particule>& particules_vector, double dt,
                                    const std::vector<double>& fx_old,
                                    const std::vector<double>& fy_old,
                                    const std::vector<double>& fz_old) {
    for (std::size_t i = 0; i < particules_vector.size(); ++i) {

        double m = particules_vector[i].getMasse();

        double x = particules_vector[i].getX() + dt * particules_vector[i].getVx()
                   + 0.5 * dt * dt * fx_old[i] / m;
        double y = particules_vector[i].getY() + dt * particules_vector[i].getVy()
                   + 0.5 * dt * dt * fy_old[i] / m;
        double z = particules_vector[i].getZ() + dt * particules_vector[i].getVz()
                   + 0.5 * dt * dt * fz_old[i] / m;

        particules_vector[i].setPosition(Vector(x, y, z));
    }
}

void renew_vitesses_Stormer_Verlet(std::vector<Particule>& particules_vector, double dt,
                                    const std::vector<double>& fx_old,
                                    const std::vector<double>& fy_old,
                                    const std::vector<double>& fz_old) {
    for (std::size_t i = 0; i < particules_vector.size(); ++i) {

        double m = particules_vector[i].getMasse();

        double vx = particules_vector[i].getVx()
                    + 0.5 * dt / m * (fx_old[i] + particules_vector[i].getFx());
        double vy = particules_vector[i].getVy()
                    + 0.5 * dt / m * (fy_old[i] + particules_vector[i].getFy());
        double vz = particules_vector[i].getVz()
                    + 0.5 * dt / m * (fz_old[i] + particules_vector[i].getFz());

        particules_vector[i].setVitesse(Vector(vx, vy, vz));
    }
}

// Remet a zero puis recalcule la force de chaque particule (necessaire car
// renew_gravitational_force_dxdydz_i accumule dans force au lieu de l'ecraser).
void recalculer_forces(std::vector<Particule>& particules_vector) {
    for (std::size_t i = 0; i < particules_vector.size(); ++i) {
        particules_vector[i].setForce(Vector(0, 0, 0));
    }
    for (std::size_t i = 0; i < particules_vector.size(); ++i) {
        renew_gravitational_force_dxdydz_i(particules_vector, i);
    }
}

int main() {
    // INITIALISATION
    int Nombre_particules = 16;
    auto start = std::chrono::steady_clock::now();

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    std::vector<Particule> listeParticule;
    listeParticule.reserve(Nombre_particules); // alloue la memoire en avance, pour ne pas la reallouer apres

    for (int i = 0; i < Nombre_particules; ++i) {

        double masse = dist(mt);

        double x = dist(mt);
        double y = dist(mt);
        double z = dist(mt);

        double vx = dist(mt);
        double vy = dist(mt);
        double vz = dist(mt);

        Particule p(x, y, z, vx, vy, vz, masse, i, 0, 0, 0, 0);
        listeParticule.push_back(p);
    }

    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";

    // ALGORITHME
    double dt = 0.015;
    double tmax = 468.5;

    std::vector<double> fx_old(Nombre_particules);
    std::vector<double> fy_old(Nombre_particules);
    std::vector<double> fz_old(Nombre_particules);

    // Forces initiales, avant d'entrer dans la boucle de temps
    recalculer_forces(listeParticule);

    for (double t = 0; t < tmax; t += dt) {

        // 1. Sauvegarde des forces courantes (F_old)
        for (int i = 0; i < Nombre_particules; ++i) {
            fx_old[i] = listeParticule[i].getFx();
            fy_old[i] = listeParticule[i].getFy();
            fz_old[i] = listeParticule[i].getFz();
        }

        // 2. Mise a jour des positions avec F_old
        renew_positions_Stormer_Verlet(listeParticule, dt, fx_old, fy_old, fz_old);

        // 3. Calcul des nouvelles forces (apres deplacement)
        recalculer_forces(listeParticule);

        // 4. Mise a jour des vitesses avec F_old et les nouvelles forces
        renew_vitesses_Stormer_Verlet(listeParticule, dt, fx_old, fy_old, fz_old);

        // Affichage
        std::cout << t;
        for (int i = 0; i < Nombre_particules; ++i) {
            std::cout << " " << listeParticule[i].getX()
                       << " " << listeParticule[i].getY()
                       << " " << listeParticule[i].getZ();
        }
        std::cout << "\n";
    }
    return 0;
}