#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <array>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <initializer_list>
#include <iostream>

// Vecteur a Dim composantes avec les operations arithmetiques de base.
template <std::size_t Dim>
class Vector {
private:
    std::array<double, Dim> data;

public:
    // Constructeur par defaut : vecteur nul.
    Vector() { data.fill(0.0); }

    // Constructeur a partir d'une liste de Dim coordonnees, ex. Vector<3>{1.0, 2.0, 3.0}.
    Vector(std::initializer_list<double> liste) {
        assert(liste.size() == Dim);
        std::size_t i = 0;
        for (double v : liste) {
            data[i++] = v;
        }
    }

    ~Vector() = default;
    Vector(const Vector& autre) = default;
    Vector(Vector&& autre) noexcept = default;
    Vector& operator=(const Vector& autre) = default;
    Vector& operator=(Vector&& autre) noexcept = default;

    // Acces indexe en lecture (0 = premier axe, ..., Dim-1 = dernier axe).
    double operator[](std::size_t i) const { return data[i]; }

    double getX() const { return data[0]; }
    double getY() const { return data[1]; }
    double getZ() const { return data[2]; }

    // Addition en place : ajoute 'autre' a ce vecteur.
    Vector& operator+=(const Vector& autre) {
        for (std::size_t i = 0; i < Dim; ++i) data[i] += autre.data[i];
        return *this;
    }

    // Soustraction en place : soustrait 'autre' de ce vecteur.
    Vector& operator-=(const Vector& autre) {
        for (std::size_t i = 0; i < Dim; ++i) data[i] -= autre.data[i];
        return *this;
    }

    // Multiplication en place par le scalaire k.
    Vector& operator*=(double k) {
        for (std::size_t i = 0; i < Dim; ++i) data[i] *= k;
        return *this;
    }

    // Division en place par le scalaire k.
    Vector& operator/=(double k) {
        for (std::size_t i = 0; i < Dim; ++i) data[i] /= k;
        return *this;
    }

    // Oppose du vecteur.
    Vector operator-() const {
        Vector resultat(*this);
        for (std::size_t i = 0; i < Dim; ++i) resultat.data[i] = -resultat.data[i];
        return resultat;
    }

    // Norme euclidienne du vecteur.
    double norm() const {
        double somme = 0.0;
        for (std::size_t i = 0; i < Dim; ++i) somme += data[i]*data[i];
        return std::sqrt(somme);
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
        for (std::size_t i = 0; i < Dim; ++i) {
            if (data[i] != autre.data[i]) return false;
        }
        return true;
    }
    bool operator!=(const Vector& autre) const {
        return !(*this == autre);
    }

    // Produit scalaire.
    double operator*(const Vector& autre) const {
        double somme = 0.0;
        for (std::size_t i = 0; i < Dim; ++i) somme += data[i]*autre.data[i];
        return somme;
    }

    // Affichage sous la forme "(x, y, ...)".
    friend std::ostream& operator<<(std::ostream& os, const Vector& v) {
        os << "(";
        for (std::size_t i = 0; i < Dim; ++i) {
            os << v.data[i];
            if (i + 1 < Dim) os << ", ";
        }
        os << ")";
        return os;
    }

    // Lecture depuis un flux : attend Dim nombres.
    friend std::istream& operator>>(std::istream& is, Vector& v) {
        for (std::size_t i = 0; i < Dim; ++i) is >> v.data[i];
        return is;
    }
};

#endif // VECTOR_HPP
