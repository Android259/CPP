#include "Particule.hpp"
#include <vector>
#include <random>
#include <chrono>


void renew_positions_Stormer_Verlet(std::vector<Particule>& particules_vector, double dt, double tmax){
    for (double t = 0; t < tmax; t += dt) {

        // Mise a jour des positions

        for (int i = 0; i < n; ++i) {

            double m = particules_vector[i].getMasse();

            fx_old[i] = particules_vector[i].getFx();
            fy_old[i] = particules_vector[i].getFy();
            fz_old[i] = particules_vector[i].getFz();

            double x = particules_vector[i].getX() + dt*particules_vector[i].getVx() + 0.5*dt*dt*fx_old[i] / m;
            double y = particules_vector[i].getY() + dt*particules_vector[i].getVy() + 0.5*dt*dt*fy_old[i] / m;
            double z = particules_vector[i].getZ() + dt*particules_vector[i].getVz() + 0.5*dt*dt*fz_old[i] / m;

            particules_vector[i].setX(x);
            particules_vector[i].setY(y);
            particules_vector[i].setZ(z);
        }
    }
}

void renew_vitesses_Stormer_Verlet(std::vector<Particule>& particules_vector, double dt, double tmax,
                                    double* fx_old, *double fy_old, *double fz_old){
    for (int i = 0; i < n; ++i) {

        double m = listeParticule[i].getMasse();

        double vx = listeParticule[i].getVx() + 0.5*dt/m * (fx_old[i] + listeParticule[i].getFx());
        double vy = listeParticule[i].getVy()+ 0.5*dt /m * (fy_old[i] + listeParticule[i].getFy());
        double vz = listeParticule[i].getVz() + 0.5*dt/m * (fz_old[i] + listeParticule[i].getFz());

        listeParticule[i].setVx(vx);
        listeParticule[i].setVy(vy);
        listeParticule[i].setVz(vz);
    }
}

int main() {
    // INITIALIZATION
    int Nombre_particules = 16;
    auto start = std::chrono::steady_clock::now()

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    std::vector<Particule> listeParticule(Nombre_particules); // alloue memoire pour N particules en avance, pour ne pas la reallouer apres

    for (int i = 0; i < Nombre_particules; ++i) {

        double masse = dist(mt);
        std::cout << "masse : " << dist(mt) << ”\n”;

        double x = dist(mt);
        double y = dist(mt);
        double z = dist(mt);
        std::cout << "position : (" << x << "," << y <<"," << z << ")"”\n”;

        double vx = dist(mt);
        double vy = dist(mt);
        double vz = dist(mt);
        std::cout << "vitesse : (" << x << "," << y <<"," << z << ")"”\n”;

        Particule p(x, y, z, vx, vy, vz, masse);

        listeParticule.insert(listeParticule.end(), p);
    }

    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::cout « ”elapsed time: ” « elapsed_seconds.count() « ”s\n”;



    // ALGORITME

    double dt = 0.015;
    double tmax = 468.5;

    std::vector<double> fx_old(n);
    std::vector<double> fy_old(n);
    std::vector<double> fz_old(n);

    // 1. Calcul des forces initiales
    for (int i = 0; i < Nombre_particules; ++i) {
        renew_gravitational_force_dxdydz_i(listeParticule, i);

    // 2. xyz
    renew_positions_Stormer_Verlet(listeParticule, dt, tmax);

    // 3. F old
    fx_old[i] = listeParticule[i].getFx();
    fy_old[i] = listeParticule[i].getFy();
    fz_old[i] = listeParticule[i].getFz();
    
    // Calcul des nouvelles forces
    for (int i = 0; i < n; ++i) {
        renew_gravitational_force_dxdydz_i(listeParticule, i);
    }

    // Renew vitesses
    renew_vitesses_Stormer_Verlet(listeParticule, dt, tmax, dx_old, fy_old, fz_old);

    //affichage 

    std::cout << t;

    for (int i = 0; i < n; ++i) {
        std::cout << " " << listeParticule[i].getX()
                << " " << listeParticule[i].getY();
                << " " << listeParticule[i].getZ();
    }

    std::cout << "\n";
}


}