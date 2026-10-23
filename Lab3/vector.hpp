#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cmath>
#include <iostream>
#include <stdexcept>

class Vector {
    private:
    double x, y, z;

    public:
    //constructeur
    Vector() : x(0), y(0), z(0) {}

    Vector(double x, double y, double z) :
        x(x), y(y), z(z) {}

    // Destructeur
    ~Vector() = default;

    // Constructeur par copie
    Vector(const Vector& autre):
        x(autre.x), y(autre.y), z(autre.z) {}

    // Constructeur de deplacement
    Vector(Vector&& autre) noexcept :
        x(autre.x), y(autre.y), z(autre.z) {}

    // Operateur d'affectation par copie
    Vector& operator=(const Vector& autre) {
        if (this != &autre) {
            x = autre.x; y = autre.y; z = autre.z;
        }
        return *this;
    }
    // Operateur d'affectation par deplacement
    Vector& operator=(Vector&& autre) noexcept {
        if (this != &autre) {
            x = autre.x; y = autre.y; z = autre.z;
        }
        return *this;
    }

    // Accesseurs
    double getX() const { return x; }
    double getY() const { return y; }
    double getZ() const { return z; }
    //Operateurs
    Vector& operator+=(const Vector& autre) {
        x += autre.x; y += autre.y; z += autre.z;
        return *this;
    }

    Vector& operator-=(const Vector& autre) {
        x -= autre.x; y -= autre.y; z -= autre.z;
        return *this;
    }

    Vector& operator*=(double k) {
        x *= k; y *= k; z *= k;
        return *this;
    }

    Vector& operator/=(double k) {
        if (k == 0.0) throw std::invalid_argument("division par zero");
        x /= k; y /= k; z /= k;
        return *this;
    }

    Vector operator-() const {
        return Vector(-x, -y, -z);
    }

    //Comparaison
    bool operator==(const Vector& autre) const {
        return x == autre.x && y == autre.y && z == autre.z;
    }
    bool operator!=(const Vector& autre) const {
        return !(*this == autre);
    }

    //Produit scalaire
    double operator*(const Vector& autre) const {
        return x * autre.x + y * autre.y + z * autre.z;
    }

    // Norme euclidienne
    double norm() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    //Affichage
    friend std::ostream& operator<<(std::ostream& os, const Vector& v) {
        os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
        return os;
    }
    //Operateurs de flux
    //Operateur d'entree
    friend std::istream& operator>>(std::istream& is, Vector& v) {
        is >> v.x >> v.y >> v.z;
        return is;
    }

};

// Operateurs arithmetiques hors classe
inline Vector operator+(Vector a, const Vector& b) { return a += b; }
inline Vector operator-(Vector a, const Vector& b) { return a -= b; }
inline Vector operator*(Vector v, double k) { return v *= k; }
inline Vector operator*(double k, Vector v) { return v *= k; }
inline Vector operator/(Vector v, double k) { return v /= k; }

#endif // VECTOR_HPP
