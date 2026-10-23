#include "vector_styles.hpp"
#include <chrono>
#include <iostream>
#include <vector>

void testVectorSeparate() {
    std::cout << "Tests VectorSeparate" << std::endl;
    VectorSeparate a(1, 2, 3);
    VectorSeparate b(4, 5, 6);
    int erreurs = 0;

    if (!((a + b) == VectorSeparate(5, 7, 9))) { std::cout << "echec : addition" << std::endl; erreurs++; }
    if (!((b - a) == VectorSeparate(3, 3, 3))) { std::cout << "echec : soustraction" << std::endl; erreurs++; }
    if (!((-a) == VectorSeparate(-1, -2, -3))) { std::cout << "echec : oppose" << std::endl; erreurs++; }
    if (!((a * 2) == VectorSeparate(2, 4, 6))) { std::cout << "echec : multiplication scalaire" << std::endl; erreurs++; }
    if (a.dot(b) != 1 * 4 + 2 * 5 + 3 * 6) { std::cout << "echec : produit scalaire" << std::endl; erreurs++; }
    if (!(a != b)) { std::cout << "echec : difference" << std::endl; erreurs++; }

    VectorSeparate c = a;
    c += b;
    if (!(c == VectorSeparate(5, 7, 9))) { std::cout << "echec : +=" << std::endl; erreurs++; }

    VectorSeparate d = std::move(c);  // constructeur par deplacement
    if (!(d == VectorSeparate(5, 7, 9))) { std::cout << "echec : deplacement" << std::endl; erreurs++; }

    if (erreurs == 0) {
        std::cout << "Tous les tests sont passes." << std::endl;
    } else {
        std::cout << erreurs << " test(s) ont echoue." << std::endl;
    }
}

void testVectorArray() {
    std::cout << "Tests VectorArray" << std::endl;
    VectorArray a(1, 2, 3);
    VectorArray b(4, 5, 6);
    int erreurs = 0;

    if (!((a + b) == VectorArray(5, 7, 9))) { std::cout << "echec : addition" << std::endl; erreurs++; }
    if (!((b - a) == VectorArray(3, 3, 3))) { std::cout << "echec : soustraction" << std::endl; erreurs++; }
    if (!((-a) == VectorArray(-1, -2, -3))) { std::cout << "echec : oppose" << std::endl; erreurs++; }
    if (!((a * 2) == VectorArray(2, 4, 6))) { std::cout << "echec : multiplication scalaire" << std::endl; erreurs++; }
    if (a.dot(b) != 1 * 4 + 2 * 5 + 3 * 6) { std::cout << "echec : produit scalaire" << std::endl; erreurs++; }
    if (!(a != b)) { std::cout << "echec : difference" << std::endl; erreurs++; }

    VectorArray c = a;
    c += b;
    if (!(c == VectorArray(5, 7, 9))) { std::cout << "echec : +=" << std::endl; erreurs++; }

    VectorArray d = std::move(c);
    if (!(d == VectorArray(5, 7, 9))) { std::cout << "echec : deplacement" << std::endl; erreurs++; }

    if (erreurs == 0) {
        std::cout << "Tous les tests sont passes." << std::endl;
    } else {
        std::cout << erreurs << " test(s) ont echoue." << std::endl;
    }
}

// Comparaison des performances

void benchmarkSeparate(int n) {
    std::vector<VectorSeparate> vecs;
    for (int i = 0; i < n; ++i) {
        vecs.push_back(VectorSeparate(i, i + 1, i + 2));
    }

    auto start = std::chrono::steady_clock::now();

    VectorSeparate acc;
    for (int iter = 0; iter < 50; ++iter) {
        for (int i = 0; i < n - 1; ++i) {
            acc += vecs[i] + vecs[i + 1] * 0.5 - vecs[i];
        }
    }

    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> time = end - start;
    std::cout << "VectorSeparate : " << time.count() << "s (resultat : " << acc.norm() << ")" << std::endl;
}

void benchmarkArray(int n) {
    std::vector<VectorArray> vecs;
    for (int i = 0; i < n; ++i) {
        vecs.push_back(VectorArray(i, i + 1, i + 2));
    }

    auto start = std::chrono::steady_clock::now();

    VectorArray acc;
    for (int iter = 0; iter < 50; ++iter) {
        for (int i = 0; i < n - 1; ++i) {
            acc += vecs[i] + vecs[i + 1] * 0.5 - vecs[i];
        }
    }

    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> time = end - start;
    std::cout << "VectorArray    : " << time.count() << "s (resultat : " << acc.norm() << ")" << std::endl;
}

int main() {
    std::cout << "=== Tests de correction ===" << std::endl;
    testVectorSeparate();
    testVectorArray();

    std::cout << std::endl << "=== Comparaison des performances ===" << std::endl;
    int n = 200000;
    benchmarkSeparate(n);
    benchmarkArray(n);

    return 0;
}