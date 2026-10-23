#include "particule_styles.hpp"
#include <chrono>
#include <iostream>
#include <vector>

// Tests de correction : une fonction par style

void testParticuleSeparate() {
    std::cout << "--- Tests ParticuleSeparate ---" << std::endl;
    std::vector<ParticuleSeparate> particules;
    particules.push_back(ParticuleSeparate(0, 0, 0, 0, 0, 0, 1.0));
    particules.push_back(ParticuleSeparate(1, 0, 0, 0, 0, 0, 1.0));
    int erreurs = 0;

    renew_gravitational_force_dxdydz_i(particules, 0);

    if (!(particules[0].getForce() == Vector(1, 0, 0))) {
        std::cout << "echec : force sur particule 0" << std::endl; erreurs++;
    }
    if (!(particules[1].getForce() == Vector(-1, 0, 0))) {
        std::cout << "echec : force sur particule 1" << std::endl; erreurs++;
    }

    if (erreurs == 0) {
        std::cout << "Tous les tests sont passes." << std::endl;
    } else {
        std::cout << erreurs << " test(s) ont echoue." << std::endl;
    }
}

void testParticuleArray() {
    std::cout << "Tests ParticuleArray" << std::endl;
    std::vector<ParticuleArray> particules;
    particules.push_back(ParticuleArray(0, 0, 0, 0, 0, 0, 1.0));
    particules.push_back(ParticuleArray(1, 0, 0, 0, 0, 0, 1.0));
    int erreurs = 0;

    renew_gravitational_force_dxdydz_i(particules, 0);

    if (!(particules[0].getForce() == Vector(1, 0, 0))) {
        std::cout << "echec : force sur particule 0" << std::endl; erreurs++;
    }
    if (!(particules[1].getForce() == Vector(-1, 0, 0))) {
        std::cout << "echec : force sur particule 1" << std::endl; erreurs++;
    }

    if (erreurs == 0) {
        std::cout << "Tous les tests sont passes." << std::endl;
    } else {
        std::cout << erreurs << " test(s) ont echoue." << std::endl;
    }
}

void testParticulePointer() {
    std::cout << "Tests ParticulePointer" << std::endl;
    std::vector<ParticulePointer> particules;
    particules.push_back(ParticulePointer(0, 0, 0, 0, 0, 0, 1.0));
    particules.push_back(ParticulePointer(1, 0, 0, 0, 0, 0, 1.0));
    int erreurs = 0;

    renew_gravitational_force_dxdydz_i(particules, 0);

    if (!(particules[0].getForce() == Vector(1, 0, 0))) {
        std::cout << "echec : force sur particule 0" << std::endl; erreurs++;
    }
    if (!(particules[1].getForce() == Vector(-1, 0, 0))) {
        std::cout << "echec : force sur particule 1" << std::endl; erreurs++;
    }

    // Copie et deplacement doivent donner le meme resultat (gere la memoire propre)
    ParticulePointer a(2, 0, 0, 0, 0, 0, 3.0);
    ParticulePointer b = a;               // copie
    ParticulePointer c = std::move(a);    // deplacement
    if (!(b.getPosition() == c.getPosition())) {
        std::cout << "echec : copie / deplacement" << std::endl; erreurs++;
    }

    if (erreurs == 0) {
        std::cout << "Tous les tests sont passes." << std::endl;
    } else {
        std::cout << erreurs << " test(s) ont echoue." << std::endl;
    }
}

// ------------------------------------------------------------
// Comparaison des performances : meme boucle de force gravitationnelle
// que dans Stormer-Verlet, pour chaque style, avec chrono::steady_clock
// ------------------------------------------------------------

void benchmarkSeparate(int n) {
    std::vector<ParticuleSeparate> particules;
    for (int i = 0; i < n; ++i) {
        particules.push_back(ParticuleSeparate(i + 1, i + 2, i + 3, 0, 0, 0, 1.0));
    }

    auto start = std::chrono::steady_clock::now();

    for (int i = 0; i < n; ++i) {
        renew_gravitational_force_dxdydz_i(particules, i);
    }

    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "ParticuleSeparate : " << elapsed_seconds.count()
               << "s (force[0] = " << particules[0].getForce() << ")" << std::endl;
}

void benchmarkArray(int n) {
    std::vector<ParticuleArray> particules;
    for (int i = 0; i < n; ++i) {
        particules.push_back(ParticuleArray(i + 1, i + 2, i + 3, 0, 0, 0, 1.0));
    }

    auto start = std::chrono::steady_clock::now();

    for (int i = 0; i < n; ++i) {
        renew_gravitational_force_dxdydz_i(particules, i);
    }

    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "ParticuleArray    : " << elapsed_seconds.count()
               << "s (force[0] = " << particules[0].getForce() << ")" << std::endl;
}

void benchmarkPointer(int n) {
    std::vector<ParticulePointer> particules;
    for (int i = 0; i < n; ++i) {
        particules.push_back(ParticulePointer(i + 1, i + 2, i + 3, 0, 0, 0, 1.0));
    }

    auto start = std::chrono::steady_clock::now();

    for (int i = 0; i < n; ++i) {
        renew_gravitational_force_dxdydz_i(particules, i);
    }

    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "ParticulePointer  : " << elapsed_seconds.count()
               << "s (force[0] = " << particules[0].getForce() << ")" << std::endl;
}

int main() {
    std::cout << "=== Tests de correction ===" << std::endl;
    testParticuleSeparate();
    testParticuleArray();
    testParticulePointer();

    std::cout << std::endl << "=== Comparaison des performances ===" << std::endl;
    std::cout << "(boucle de force gravitationnelle, comme dans Stormer-Verlet)" << std::endl;
    int n = 2000;
    benchmarkSeparate(n);
    benchmarkArray(n);
    benchmarkPointer(n);

    return 0;
}