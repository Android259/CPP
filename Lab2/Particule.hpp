#ifndef PARTICULE_HPP
#define PARTICULE_HPP

#include "vector.hpp"

class Particule {
private:
    Vector position;
    Vector vitesse;
    double masse;
    int id;
    int type;
    Vector force;

public:
    // Constructeur
    Particule(double masse)
        : position(0, 0, 0), vitesse(0, 0, 0), masse(masse),
          id(0), type(0), force(0, 0, 0) {}

    Particule(double x, double y, double z,
              double vx, double vy, double vz,
              double masse, int id, int type,
              double fx, double fy, double fz)
        : position(x, y, z), vitesse(vx, vy, vz), masse(masse),
          id(id), type(type), force(fx, fy, fz) {}

    double getMasse() const { return masse; }
    void setMasse(double m) { masse = m; }

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