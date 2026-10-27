#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cmath>
#include <iostream>

// Vecteur 3D (x, y, z) avec les operations arithmetiques de base.
class Vector {
private:
    double x, y, z;

public:
    // Constructeur par defaut : vecteur nul (0, 0, 0).
    Vector() : x(0), y(0), z(0) {}

    // Constructeur a partir des trois coordonnees.
    Vector(double x, double y, double z) :
        x(x), y(y), z(z) {}

    // Destructeur par defaut (aucune ressource a liberer).
    ~Vector() = default;

    // Constructeur par copie.
    Vector(const Vector& autre) :
        x(autre.x), y(autre.y), z(autre.z) {}

    // Constructeur de deplacement.
    Vector(Vector&& autre) noexcept :
        x(autre.x), y(autre.y), z(autre.z) {}

    // Operateur d'affectation par copie.
    Vector& operator=(const Vector& autre) {
        if (this != &autre) {
            x = autre.x; y = autre.y; z = autre.z;
        }
        return *this;
    }

    // Operateur d'affectation par deplacement.
    Vector& operator=(Vector&& autre) noexcept {
        if (this != &autre) {
            x = autre.x; y = autre.y; z = autre.z;
        }
        return *this;
    }

    // Accesseurs en lecture pour chaque coordonnee.
    double getX() const { return x; }
    double getY() const { return y; }
    double getZ() const { return z; }

    // Acces indexe (0=x, 1=y, 2=z), pour le code generique en dimension
    // (boucles for d in [0, dimension) sur les axes, cf. Univers).
    double operator[](int i) const {
        return i == 0 ? x : (i == 1 ? y : z);
    }

    // Addition en place : ajoute 'autre' a ce vecteur.
    Vector& operator+=(const Vector& autre) {
        x += autre.x; y += autre.y; z += autre.z;
        return *this;
    }

    // Soustraction en place : soustrait 'autre' de ce vecteur.
    Vector& operator-=(const Vector& autre) {
        x -= autre.x; y -= autre.y; z -= autre.z;
        return *this;
    }

    // Multiplication en place par le scalaire k.
    Vector& operator*=(double k) {
        x *= k; y *= k; z *= k;
        return *this;
    }

    // Division en place par le scalaire k.
    Vector& operator/=(double k) {
        x /= k; y /= k; z /= k;
        return *this;
    }

    // Oppose du vecteur : (-x, -y, -z).
    Vector operator-() const {
        return Vector(-x, -y, -z);
    }

    // Norme euclidienne du vecteur.
    double norm() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    // Operateurs arithmetiques binaires (+, -, *, / entre vecteur et scalaire).
    // Definis en amis directement dans la classe plutot qu'avec le mot-cle
    // "inline" : une fonction amie definie a l'interieur de la classe est
    // implicitement inline, ce qui evite les erreurs de redefinition quand
    // ce fichier est inclus depuis plusieurs .cpp.
    friend Vector operator+(Vector a, const Vector& b) { return a += b; }
    friend Vector operator-(Vector a, const Vector& b) { return a -= b; }
    friend Vector operator*(Vector v, double k) { return v *= k; }
    friend Vector operator*(double k, Vector v) { return v *= k; }
    friend Vector operator/(Vector v, double k) { return v /= k; }

    // Egalite / difference terme a terme.
    bool operator==(const Vector& autre) const {
        return x == autre.x && y == autre.y && z == autre.z;
    }
    bool operator!=(const Vector& autre) const {
        return !(*this == autre);
    }

    // Produit scalaire.
    double operator*(const Vector& autre) const {
        return x * autre.x + y * autre.y + z * autre.z;
    }

    // Affichage sous la forme "(x, y, z)".
    friend std::ostream& operator<<(std::ostream& os, const Vector& v) {
        os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
        return os;
    }

    // Lecture depuis un flux : attend trois nombres "x y z".
    friend std::istream& operator>>(std::istream& is, Vector& v) {
        is >> v.x >> v.y >> v.z;
        return is;
    }
};

#endif // VECTOR_HPP
