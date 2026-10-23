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
};

#endif // PARTICULE_HPP
