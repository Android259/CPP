#ifndef VECTEUR_HPP
#define VECTEUR_HPP

#include <cmath>
#include <iostream>
#include <list>
#include <assert.h> // pour vérifier la taille de la liste lors de la création d'un vecteur

template <int N>
class Vecteur {
   private:
    double data[N];

   public:
    /// @brief le constructeur par défaut créer un vecteur sans garanti sur les valeurs
    Vecteur() {}

    /// @brief créer un vecteur avec toutes les composantes qui valent une certaine valeur
    /// @param valeur la valeur de toutes les composantes
    Vecteur(double valeur) {
        for (int i = 0; i < N; i++) {
            data[i] = valeur;
        }
    }

    /// @brief créer un vecteur a partir d'un tableau de la même dimension
    /// @param arr le tableau qui donne les valeurs des composantes
    Vecteur(const double (&arr)[N]) {
        for (int i = 0; i < N; i++) {
            data[i] = arr[i];
        }
    }

    /// @todo peut-être faire un constructeur à partir d'un vecteur

    /// @brief créer un vecteur a partir d'une liste
    /// @param liste liste qui donne la valeur des composantes
    /// @warning La fonction arrete le programme si la dimension ne correspond pas à la taille de la liste
    /// @todo peut être changer le assert pour une vérification à la compilation sachant que l'utilisation principale est pour avoir vecteur<...> ... = {...,...};
    Vecteur(std::initializer_list<double> liste) {
        assert(N == liste.size());
        std::copy(liste.begin(), liste.end(), data);
    }

    /// @brief opérateur d'égalité
    /// @param v1 le premier vecteur
    /// @param v2 le deuxieme vecteur
    /// @return true si les deux vecteur sont égaux et faux sinon
    friend bool operator==(const Vecteur& v1, const Vecteur& v2){
        for (int i = 0; i < N; i++){
            if (v1[i] != v2[i]){return false;}
        }
        return true;
    }

    /// @brief opérateur d'égalité
    /// @param v1 le premier vecteur
    /// @param v2 le deuxieme vecteur
    /// @return true si les deux vecteur sont égaux et faux sinon
    friend bool operator!=(const Vecteur& v1, const Vecteur& v2){
        return !(v1==v2);
    }

    /// @brief opérateur d'assignation
    /// @param source le vecteur que l'on veut copier
    /// @return le vecteur qui a été modifié
    Vecteur& operator=(const Vecteur& source) {
        for (int i = 0; i < N; i++) {
            data[i] = source.data[i];
        }
        return *this;  // Retourne l'objet lui-même
    }

    /// @brief opérateur d'incrementation
    /// @param autre le vecteur que l'on veut ajouter
    /// @return le vecteur auquel l'autre vecteur a été ajouté
    Vecteur operator+=(const Vecteur& autre) {
        for (int i = 0; i < N; i++) {
            data[i] += autre.data[i];
        }
        return *this;
    }

    /// @brief multiplie le vecteur par un scalaire (chacune des composantes est multipliée)
    /// @param scalaire la valeur par laquelle on veut multiplier le vecteur
    /// @return le vecteur qui a été modifié
    Vecteur operator*=(double scalaire) {
        for (int i = 0; i < N; i++) {
            data[i] *= scalaire;
        }
        return *this;
    }

    /// @brief soustraction d'un autre vecteur
    /// @param autre le vecteur qui soustrait
    /// @return le vecteur qui a été modifié
    Vecteur operator-=(Vecteur autre) {
        *this += (-1) * autre;
        return *this;
    }

    /// @brief Addition de deux vecteurs(additionne composante par composante)
    /// @param a le premier vecteur
    /// @param b le second vecteur
    /// @return la somme des deux vecteurs
    friend Vecteur operator+(Vecteur a, const Vecteur& b) { return a += b; }

    /// @brief Soustration de deux vecteurs(soustrait composante par composante)
    /// @param a le premier vecteur
    /// @param b le second vecteur
    /// @return la soustraction des deux vecteurs
    friend Vecteur operator-(Vecteur a, const Vecteur& b) { return a -= b; }

    /// @brief Multiplication scalaire d'un scalaire et d'un vecteur
    /// @param s le scalaire
    /// @param v le vecteur
    /// @return le vecteur multiplié par le scalaire
    friend Vecteur operator*(double s, Vecteur v) { return v *= s; }

    /// @brief Affichage d'un vecteur dans un flux
    /// @param os le flux de sortie
    /// @param v le vecteur
    /// @return le flux de sortie après affichage du vecteur
    friend std::ostream& operator<<(std::ostream& os, const Vecteur& v) {
        os << "(";
        for (int i = 0; i < N - 1; i++) {
            os << v.data[i] << ",";
        }
        os << v.data[N - 1] << ")";
        return os;
    }

    /// @brief Récupération d'un vecteur dans un flux
    /// @param is le flux d'entrée
    /// @param v le vecteur
    /// @return le flux d'entrée après récupération du vecteur
    friend std::istream& operator>>(std::istream& is, Vecteur& v) {
        for (int i = 0; i < N; i++) {
            is >> v.data[i];
        }
        return is;
    }

    /// @brief Accès mathématique( () : premier élément à l'indice 1) en lecture seule
    /// @param i l'indice désiré
    /// @return la valeur de la composante i (en commencant par 1) du vecteur
    double operator()(int i) const { return data[i - 1]; }

    /// @brief Accès mathématique( () : premier élément à l'indice 1) en lecture et écriture
    /// @param i l'indice désiré
    /// @return Une valeur modifiable de la composante i (en commencant par 1) du vecteur
    double& operator()(int i) { return data[i - 1]; }
    
    /// @brief Accès c++( [] : premier élément à l'indice 0) en lecture seule
    /// @param i l'indice désiré
    /// @return la valeur de la composante i (en commencant par 0) du vecteur
    double operator[](int i) const { return data[i]; }

    /// @brief Accès c++( [] : premier élément à l'indice 0) en lecture et écriture
    /// @param i l'indice désiré
    /// @return Une valeur modifiable de la composante i (en commencant par 0) du vecteur
    double& operator[](int i) { return data[i]; }
    
    /// @brief Calcule le produit scalaire entre deux vecteurs (le produit scalaire euclidien (somme du produit composante par composante))
    /// @param v1 le premier vecteur
    /// @param v2 le second vecteur
    /// @return Le produit scalaire des deux vecteurs
    friend double produit_scalaire_euclidien(const Vecteur<N>& v1,
                                             const Vecteur<N>& v2) {
        double somme = 0;
        for (int i = 0; i < N; i++) {
            somme += v1.data[i] * v2.data[i];
        }
        return somme;
    }

    /// @brief Calcule la norme 2 (norme euclidienne) d'un vecteur
    /// @return La norme 2 du vecteur
    double norme_2() const {
        return sqrt(produit_scalaire_euclidien(*this, *this));
    }

    /// @brief Calcule la distance (euclidienne) entre deux vecteur
    /// @param v1 le premier vecteur
    /// @param v2 le second vecteur
    /// @return La distance entre les deux vecteurs
    friend double distance_euclidienne(const Vecteur<N>& v1,
                                       const Vecteur<N>& v2) {
        Vecteur v = v1 - v2;
        return v.norme_2();
    }
};
#endif