#ifndef PARTICULES_HPP
#define PARTICULES_HPP

#include <cmath>
#include <iostream>

#include "vecteur.hpp"

template <int Dim>
class Particule {
   private:
    double masse;  // attribut caché, inaccessible depuis l'extérieur
    Vecteur<Dim> position;
    Vecteur<Dim> vitesse;
    int identifiant;
    int categorie;
    Vecteur<Dim> force;

   public:
    /// @brief Créer une particule avec sa masse, ses position, vitesse, force, son identifiant et sa catégorie
    /// @param masse la masse de la particule
    /// @param position la position de la particule
    /// @param vitesse la vitesse de la particule
    /// @param force la somme des forces sur la particule
    /// @param identifiant l'identifiant de la particule
    /// @param categorie la categorie de la particule
    Particule(double masse, const Vecteur<Dim>& position,
              const Vecteur<Dim>& vitesse, const Vecteur<Dim>& force,
              int identifiant,
              int categorie) {  // constructeur
        setMasse(masse);
        setPosition(position);
        setVitesse(vitesse);
        setForce(force);
        setIdentifiant(identifiant);
        setCategorie(categorie);
    }

    /// @brief Donne la masse de la particule
    /// @return la masse de la particule
    double getMasse() const {
        return this->masse;
    }
    /// @brief Modifie la masse de la particule
    /// @param m la nouvelle masse de la particule
    void setMasse(double m) {
        this->masse = m;
    }

    /// @brief Donne la position de la particule
    /// @return la position de la particule
    Vecteur<Dim> const& getPosition() const { return this->position; }
    /// @brief Modifie la position de la particule
    /// @param m la nouvelle position de la particule
    void setPosition(const Vecteur<Dim>& p) {
        this->position = p;
    }

    /// @brief Donne la vitesse de la particule
    /// @return la vitesse de la particule
    Vecteur<Dim> const& getVitesse() const { return this->vitesse; }
    /// @brief Modifie la vitesse de la particule
    /// @param m la nouvelle vitesse de la particule
    void setVitesse(const Vecteur<Dim>& v) {
        this->vitesse = v;
    }

    /// @brief Donne la force sur la particule
    /// @return la force sur la particule
    Vecteur<Dim> const& getForce() const { return this->force; }
    /// @brief Modifie la force sur la particule
    /// @param m la nouvelle force sur la particule
    void setForce(const Vecteur<Dim>& f) {
        this->force = f;
    }

    /// @brief Donne l'identifiant de la particule
    /// @return l'identifiant de la particule
    int getIdentifiant() const { return this->identifiant; }
    /// @brief Modifie l'identifiant de la particule
    /// @param m le nouveau idientifiant de la particule
    void setIdentifiant(int id) { this->identifiant = id; }

    /// @brief Donne la categorie de la particule
    /// @return la categorie de la particule
    int getCategorie() const { return this->categorie; }
    /// @brief Modifie la categorie de la particule
    /// @param m la nouvelle categorie de la particule
    void setCategorie(int c) { this->categorie = c; }

    /// @brief Affichage de la particule dans un flux
    /// @param os le flux de sortie
    /// @param p la particule
    /// @return le flux de sortie après affichage de la particule
    friend std::ostream& operator<<(std::ostream& os, const Particule& p) {
        os << "{";
        os << "masse = " << p.masse << ";";
        os << "position = " << p.position << ";";
        os << "vitesse = " << p.vitesse << ";";
        os << "force = " << p.force << ";";
        os << "identifiant = " << p.identifiant << ";";
        os << "categorie = " << p.categorie << ";";
        os << "}";
        return os;
    }

    /// @brief Récupération d'une particule dans un flux
    /// @param is le flux d'entrée
    /// @param p la particule
    /// @return le flux d'entrée après récupération de la particule
    friend std::istream& operator>>(std::istream& is, Particule& p) {
        is >> p.masse;
        is >> p.position;
        is >> p.vitesse;
        is >> p.force;
        is >> p.identifiant;
        is >> p.categorie;
        return is;
    }

    /// @brief Calcule le potentiel gravitationnel entre deux particules
    /// @param p1 la première particule
    /// @param p2 la seconde particule
    /// @return le potentiel entre les deux particules
    friend double potentiel_gravitationelle(const Particule& p1,
                                              const Particule& p2) {
        double dist = distance_euclidienne(p1.position, p2.position);
        if (dist == 0) {
            exit(EXIT_FAILURE);
        }
        return -p1.masse * p2.masse / dist;
    }

    /// @brief Calcule la force gravitationnelle entre deux particules
    /// @param p1 la première particule
    /// @param p2 la seconde particule
    /// @return la force gravitatonnelle entre les deux particules
    friend Vecteur<Dim> force_gravitationelle(const Particule& p1,
                                              const Particule& p2) {
        double dist = distance_euclidienne(p1.position, p2.position);
        if (dist == 0) {
            exit(EXIT_FAILURE);
        }
        double valeur_force_sur_distance =
            p1.masse * p2.masse / dist / dist / dist;
        return valeur_force_sur_distance * (p2.position - p1.position);
    }

    /// @brief Calcule le potentiel de Lennard-Jones entre deux particules
    /// @param epsilon le terme epsilon du potentiel
    /// @param sigma le terme sigma du potentiel
    /// @param p1 la première particule
    /// @param p2 la seconde particule
    /// @return le potentiel entre les deux particules
    friend double potentiel_lennard_jones(double epsilon, double simga, const Particule& p1,
                                              const Particule& p2) {
        double dist = distance_euclidienne(p1.position, p2.position);
        return 4 * epsilon * pow(simga / dist, 6) * (pow(simga / dist, 6) - 1);
    }

    /// @brief Calcule la force de Lennard-Jones entre deux particules
    /// @param epsilon le terme epsilon du potentiel
    /// @param sigma le terme sigma du potentiel
    /// @param p1 la première particule
    /// @param p2 la seconde particule
    /// @return la force de Lennard-Jones entre les deux particules
    friend Vecteur<Dim> force_lennard_jones(double epsilon, double simga, const Particule& p1,
                                              const Particule& p2) {
        double dist = distance_euclidienne(p1.position, p2.position);
        if (dist == 0) {
            exit(EXIT_FAILURE);
        }
        double valeur_force_sur_distance =
            24.0 * epsilon * pow(simga / dist, 6) * (1 - 2 * pow(simga / dist, 6)) / dist / dist;
        return valeur_force_sur_distance * (p2.position - p1.position);
    }

    /// @brief Calcule l'énergie cinétique d'une particule
    /// @return l'énergie cinétique de la particule
    double energie_cinetique(){
        return (1.0 / 2.0) * this->masse * produit_scalaire_euclidien(this->vitesse, this->vitesse);
    }
};

#endif