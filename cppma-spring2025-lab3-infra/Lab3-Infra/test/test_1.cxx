// Question 3 : tests qui verifient chacune des methodes de la classe Vector.

#include "Vector.hpp"
#include <iostream>

int erreurs = 0;

void verifier(bool condition, const char* nom) {
    if (!condition) {
        std::cout << "echec : " << nom << std::endl;
        ++erreurs;
    }
}

int main() {
    Vector a(1, 2, 3);
    Vector b(4, 5, 6);

    // constructeurs / accesseurs
    verifier(Vector() == Vector(0, 0, 0), "constructeur par defaut");
    verifier(a.getX() == 1 && a.getY() == 2 && a.getZ() == 3, "accesseurs");

    // operateurs arithmetiques
    verifier((a + b) == Vector(5, 7, 9), "addition");
    verifier((b - a) == Vector(3, 3, 3), "soustraction");
    verifier((-a) == Vector(-1, -2, -3), "oppose");
    verifier((a * 2) == Vector(2, 4, 6), "multiplication scalaire (v*k)");
    verifier((2 * a) == Vector(2, 4, 6), "multiplication scalaire (k*v)");
    verifier((b / 2) == Vector(2, 2.5, 3), "division scalaire");
    verifier((a * b) == 1 * 4 + 2 * 5 + 3 * 6, "produit scalaire");
    verifier(a != b, "difference");
    verifier(Vector(3, 4, 0).norm() == 5, "norme");

    // operateurs composes
    Vector c = a;
    c += b;
    verifier(c == Vector(5, 7, 9), "operateur +=");
    c -= b;
    verifier(c == a, "operateur -=");
    c *= 2;
    verifier(c == Vector(2, 4, 6), "operateur *=");
    c /= 2;
    verifier(c == a, "operateur /=");

    if (erreurs == 0) {
        std::cout << "Tous les tests sont passes." << std::endl;
    } else {
        std::cout << erreurs << " test(s) ont echoue." << std::endl;
    }

    return erreurs == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
