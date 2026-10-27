#ifndef PARTICULE_HPP
#define PARTICULE_HPP

#include "../include/Vector.hpp"

class Particule {
private:
    Vector position;
    Vector vitesse;
    double masse;
    double invMasse; // = 1.0 / masse, mise en cache pour eviter de diviser par masse a chaque pas de temps
    int id;
    int type;
    Vector force;

public:
    // Constructeur
    Particule(double masse)
        : position(0, 0, 0), vitesse(0, 0, 0), masse(masse), invMasse(1.0 / masse),
          id(0), type(0), force(0, 0, 0) {}

    Particule(double x, double y, double z,
              double vx, double vy, double vz,
              double masse, int id, int type,
              double fx, double fy, double fz)
        : position(x, y, z), vitesse(vx, vy, vz), masse(masse), invMasse(1.0 / masse),
          id(id), type(type), force(fx, fy, fz) {}

    double getMasse() const { return masse; }
    void setMasse(double m) { masse = m; invMasse = 1.0 / m; }

    // 1.0 / masse, deja calcule : a utiliser (multiplier) a la place d'une
    // division par masse dans les boucles de simulation.
    double getInvMasse() const { return invMasse; }

    int getId() const { return id; }
    void setId(int i) { id = i; }

    int getType() const { return type; }
    void setType(int t) { type = t; }

    // Accesseurs vectoriels
    const Vector& getPosition() const { return position; }
    void setPosition(const Vector& p) { position = p; }

    const Vector& getVitesse() const { return vitesse; }
    void setVitesse(const Vector& v) { vitesse = v; }

    const Vector& getForce() const { return force; }
    void setForce(const Vector& f) { force = f; }

    // Ajoute f a la force courante, sans passer par getForce()+setForce()
    // (evite de construire une copie intermediaire du vecteur force).
    void ajouterForce(const Vector& f) { force += f; }

    // Accesseurs par composante, conserves pour compatibilite
    double getX() const { return position.getX(); }
    double getY() const { return position.getY(); }
    double getZ() const { return position.getZ(); }

    double getVx() const { return vitesse.getX(); }
    double getVy() const { return vitesse.getY(); }
    double getVz() const { return vitesse.getZ(); }

    double getFx() const { return force.getX(); }
    double getFy() const { return force.getY(); }
    double getFz() const { return force.getZ(); }
};

#endif // PARTICULE_HPP