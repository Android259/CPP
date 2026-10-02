#include "Particule.hpp"
#include <vecctor>
#include <random>
#include <chrono>
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
    // STORMER-VERLET
    std::list<double> F_ijs;
    for (int i=0;i < Nombre_particules; ++i) {
        double F = 
    }
}