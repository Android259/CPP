#ifndef PARTICULE_STYLES_HPP
#define PARTICULE_STYLES_HPP
 
#include "vector.hpp"
#include <array>
#include <vector>
 
// Style 1 : membres Vector separes (position, vitesse, force)

class ParticuleSeparate {
    Vector position;
    Vector vitesse;
    Vector force;
    double masse;
 
public:
    ParticuleSeparate(double x, double y, double z,
                       double vx, double vy, double vz,
                       double masse)
        : position(x, y, z), vitesse(vx, vy, vz), force(0, 0, 0), masse(masse) {}
 
    double getMasse() const { return masse; }
    const Vector& getPosition() const { return position; }
    const Vector& getVitesse() const { return vitesse; }
    const Vector& getForce() const { return force; }
 
    void setPosition(const Vector& p) { position = p; }
    void setVitesse(const Vector& v) { vitesse = v; }
    void setForce(const Vector& f) { force = f; }
};
 
inline double gravitational_force_ij(const std::vector<ParticuleSeparate>& particules, int i, int j,
                               const Vector& d) {
    double r = d.norm();
    return particules[i].getMasse() * particules[j].getMasse() / (r * r * r);
}
 
inline void renew_gravitational_force_dxdydz_i(std::vector<ParticuleSeparate>& particules, int i) {
    for (std::size_t j = i + 1; j < particules.size(); ++j) {
        Vector d = particules[j].getPosition() - particules[i].getPosition();
        double F_ij = gravitational_force_ij(particules, i, j, d);
        particules[i].setForce(particules[i].getForce() + d * F_ij);
        particules[j].setForce(particules[j].getForce() - d * F_ij);
    }
}

// Style 2 : std::array<Vector, 3> (0=position, 1=vitesse, 2=force)

class ParticuleArray {
    std::array<Vector, 3> d;
    double masse;
 
public:
    ParticuleArray(double x, double y, double z,
                   double vx, double vy, double vz,
                   double masse)
        : d{Vector(x, y, z), Vector(vx, vy, vz), Vector(0, 0, 0)}, masse(masse) {}
 
    double getMasse() const { return masse; }
    const Vector& getPosition() const { return d[0]; }
    const Vector& getVitesse() const { return d[1]; }
    const Vector& getForce() const { return d[2]; }
 
    void setPosition(const Vector& p) { d[0] = p; }
    void setVitesse(const Vector& v) { d[1] = v; }
    void setForce(const Vector& f) { d[2] = f; }
};
 
inline double gravitational_force_ij(const std::vector<ParticuleArray>& particules, int i, int j,
                               const Vector& d) {
    double r = d.norm();
    return particules[i].getMasse() * particules[j].getMasse() / (r * r * r);
}
 
inline void renew_gravitational_force_dxdydz_i(std::vector<ParticuleArray>& particules, int i) {
    for (std::size_t j = i + 1; j < particules.size(); ++j) {
        Vector d = particules[j].getPosition() - particules[i].getPosition();
        double F_ij = gravitational_force_ij(particules, i, j, d);
        particules[i].setForce(particules[i].getForce() + d * F_ij);
        particules[j].setForce(particules[j].getForce() - d * F_ij);
    }
}

// Style 3 : pointeurs + allocation dynamique (Vector* par champ)

class ParticulePointer {
    Vector* position;
    Vector* vitesse;
    Vector* force;
    double masse;
 
public:
    ParticulePointer(double x, double y, double z,
                      double vx, double vy, double vz,
                      double masse)
        : position(new Vector(x, y, z)), vitesse(new Vector(vx, vy, vz)),
          force(new Vector(0, 0, 0)), masse(masse) {}
 
    ~ParticulePointer() {
        delete position;
        delete vitesse;
        delete force;
    }
 
    ParticulePointer(const ParticulePointer& autre)
        : position(new Vector(*autre.position)), vitesse(new Vector(*autre.vitesse)),
          force(new Vector(*autre.force)), masse(autre.masse) {}
 
    ParticulePointer(ParticulePointer&& autre) noexcept
        : position(autre.position), vitesse(autre.vitesse), force(autre.force), masse(autre.masse) {
        autre.position = nullptr;
        autre.vitesse = nullptr;
        autre.force = nullptr;
    }
 
    ParticulePointer& operator=(const ParticulePointer& autre) {
        if (this != &autre) {
            *position = *autre.position;
            *vitesse = *autre.vitesse;
            *force = *autre.force;
            masse = autre.masse;
        }
        return *this;
    }
 
    ParticulePointer& operator=(ParticulePointer&& autre) noexcept {
        if (this != &autre) {
            delete position;
            delete vitesse;
            delete force;
            position = autre.position;
            vitesse = autre.vitesse;
            force = autre.force;
            masse = autre.masse;
            autre.position = nullptr;
            autre.vitesse = nullptr;
            autre.force = nullptr;
        }
        return *this;
    }
 
    double getMasse() const { return masse; }
    const Vector& getPosition() const { return *position; }
    const Vector& getVitesse() const { return *vitesse; }
    const Vector& getForce() const { return *force; }
 
    void setPosition(const Vector& p) { *position = p; }
    void setVitesse(const Vector& v) { *vitesse = v; }
    void setForce(const Vector& f) { *force = f; }
};
 
inline double gravitational_force_ij(const std::vector<ParticulePointer>& particules, int i, int j,
                               const Vector& d) {
    double r = d.norm();
    return particules[i].getMasse() * particules[j].getMasse() / (r * r * r);
}
 
inline void renew_gravitational_force_dxdydz_i(std::vector<ParticulePointer>& particules, int i) {
    for (std::size_t j = i + 1; j < particules.size(); ++j) {
        Vector d = particules[j].getPosition() - particules[i].getPosition();
        double F_ij = gravitational_force_ij(particules, i, j, d);
        particules[i].setForce(particules[i].getForce() + d * F_ij);
        particules[j].setForce(particules[j].getForce() - d * F_ij);
    }
}
 
#endif // PARTICULE_STYLES_HPP
