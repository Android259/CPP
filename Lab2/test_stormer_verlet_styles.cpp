#include "stormer_verlet_styles.cpp"
#include <random>
#include <chrono>
#include <iostream>

// Tests de correction : deux particules de masse 1 separees de 1,
// apres un pas de temps, doivent se rapprocher (force attractive).

void testVector() {
    std::cout << "--- Tests std::vector<Particule> ---" << std::endl;
    std::vector<Particule> c;
    c.push_back(Particule(0, 0, 0, 0, 0, 0, 1.0, 0, 0, 0, 0, 0));
    c.push_back(Particule(1, 0, 0, 0, 0, 0, 1.0, 1, 0, 0, 0, 0));
    int erreurs = 0;

    recalculer_forces(c);
    if (!(c[0].getForce() == Vector(1, 0, 0))) { std::cout << "echec : force" << std::endl; erreurs++; }

    pas_de_temps(c, 0.01);
    if (!(c[0].getX() > 0)) { std::cout << "echec : particule 0 doit avancer vers +x" << std::endl; erreurs++; }
    if (!(c[1].getX() < 1)) { std::cout << "echec : particule 1 doit reculer vers -x" << std::endl; erreurs++; }

    if (erreurs == 0) std::cout << "Tous les tests sont passes." << std::endl;
    else std::cout << erreurs << " test(s) ont echoue." << std::endl;
}

void testList() {
    std::cout << "--- Tests std::list<Particule> ---" << std::endl;
    std::list<Particule> c;
    c.push_back(Particule(0, 0, 0, 0, 0, 0, 1.0, 0, 0, 0, 0, 0));
    c.push_back(Particule(1, 0, 0, 0, 0, 0, 1.0, 1, 0, 0, 0, 0));
    int erreurs = 0;

    recalculer_forces(c);
    if (!(c.front().getForce() == Vector(1, 0, 0))) { std::cout << "echec : force" << std::endl; erreurs++; }

    pas_de_temps(c, 0.01);
    if (!(c.front().getX() > 0)) { std::cout << "echec : particule 0 doit avancer vers +x" << std::endl; erreurs++; }
    if (!(c.back().getX() < 1)) { std::cout << "echec : particule 1 doit reculer vers -x" << std::endl; erreurs++; }

    if (erreurs == 0) std::cout << "Tous les tests sont passes." << std::endl;
    else std::cout << erreurs << " test(s) ont echoue." << std::endl;
}

void testDeque() {
    std::cout << "--- Tests std::deque<Particule> ---" << std::endl;
    std::deque<Particule> c;
    c.push_back(Particule(0, 0, 0, 0, 0, 0, 1.0, 0, 0, 0, 0, 0));
    c.push_back(Particule(1, 0, 0, 0, 0, 0, 1.0, 1, 0, 0, 0, 0));
    int erreurs = 0;

    recalculer_forces(c);
    if (!(c[0].getForce() == Vector(1, 0, 0))) { std::cout << "echec : force" << std::endl; erreurs++; }

    pas_de_temps(c, 0.01);
    if (!(c[0].getX() > 0)) { std::cout << "echec : particule 0 doit avancer vers +x" << std::endl; erreurs++; }
    if (!(c[1].getX() < 1)) { std::cout << "echec : particule 1 doit reculer vers -x" << std::endl; erreurs++; }

    if (erreurs == 0) std::cout << "Tous les tests sont passes." << std::endl;
    else std::cout << erreurs << " test(s) ont echoue." << std::endl;
}

// Question 3 : comparer les performances de chacune des collections
// pour 64, 128, ..., 2048, ... particules, avec chrono.

std::vector<Particule> genererParticulesAleatoires(int n) {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    std::vector<Particule> particules;
    for (int i = 0; i < n; ++i) {
        double x = dist(mt), y = dist(mt), z = dist(mt);
        double vx = dist(mt), vy = dist(mt), vz = dist(mt);
        double masse = dist(mt) + 0.1; // masse > 0
        particules.push_back(Particule(x, y, z, vx, vy, vz, masse, i, 0, 0, 0, 0));
    }
    return particules;
}

void benchmarkVector(int n) {
    std::vector<Particule> c = genererParticulesAleatoires(n);

    auto start = std::chrono::steady_clock::now();

    pas_de_temps(c, 0.01);

    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "vector  N=" << n << " : elapsed time: " << elapsed_seconds.count() << "s\n";
}

void benchmarkList(int n) {
    std::vector<Particule> depart = genererParticulesAleatoires(n);
    std::list<Particule> c(depart.begin(), depart.end());

    auto start = std::chrono::steady_clock::now();

    pas_de_temps(c, 0.01);

    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "list    N=" << n << " : elapsed time: " << elapsed_seconds.count() << "s\n";
}

void benchmarkDeque(int n) {
    std::vector<Particule> depart = genererParticulesAleatoires(n);
    std::deque<Particule> c(depart.begin(), depart.end());

    auto start = std::chrono::steady_clock::now();

    pas_de_temps(c, 0.01);

    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "deque   N=" << n << " : elapsed time: " << elapsed_seconds.count() << "s\n";
}

int main() {
    std::cout << "=== Tests de correction ===" << std::endl;
    testVector();
    testList();
    testDeque();

    std::cout << std::endl << "Question 3 : performances par pas de temps" << std::endl;
    std::cout << "(un seul pas_de_temps mesure, car c'est ce qui se repete a chaque iteration)" << std::endl;

    for (int n = 64; n <= 2048; n *= 2) {
        std::cout << "-- N = " << n << " --" << std::endl;
        benchmarkVector(n);
        benchmarkList(n);
        benchmarkDeque(n);
    }

    return 0;
}