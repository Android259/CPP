#ifndef STORMER_VERLET_STYLES_CPP
#define STORMER_VERLET_STYLES_CPP

#include "Particule.hpp"
#include <vector>
#include <list>
#include <deque>
#include <cstddef>

double gravitational_force_ij(double mi, double mj, const Vector& d) {
    double r = d.norm();
    return mi * mj / (r * r * r);
}

// std::vector<Particule>

void recalculer_forces(std::vector<Particule>& c) {
    for (auto& p : c) p.setForce(Vector(0, 0, 0));

    for (auto it1 = c.begin(); it1 != c.end(); ++it1) {
        auto it2 = it1;
        ++it2;
        for (; it2 != c.end(); ++it2) {
            Vector d = it2->getPosition() - it1->getPosition();
            double F_ij = gravitational_force_ij(it1->getMasse(), it2->getMasse(), d);
            it1->setForce(it1->getForce() + d * F_ij);
            it2->setForce(it2->getForce() - d * F_ij);
        }
    }
}

void avancer_positions(std::vector<Particule>& c, double dt,
                        const std::vector<double>& fx_old,
                        const std::vector<double>& fy_old,
                        const std::vector<double>& fz_old) {
    std::size_t i = 0;
    for (auto it = c.begin(); it != c.end(); ++it, ++i) {
        double m = it->getMasse();
        double x = it->getX() + dt * it->getVx() + 0.5 * dt * dt * fx_old[i] / m;
        double y = it->getY() + dt * it->getVy() + 0.5 * dt * dt * fy_old[i] / m;
        double z = it->getZ() + dt * it->getVz() + 0.5 * dt * dt * fz_old[i] / m;
        it->setPosition(Vector(x, y, z));
    }
}

void avancer_vitesses(std::vector<Particule>& c, double dt,
                       const std::vector<double>& fx_old,
                       const std::vector<double>& fy_old,
                       const std::vector<double>& fz_old) {
    std::size_t i = 0;
    for (auto it = c.begin(); it != c.end(); ++it, ++i) {
        double m = it->getMasse();
        double vx = it->getVx() + 0.5 * dt / m * (fx_old[i] + it->getFx());
        double vy = it->getVy() + 0.5 * dt / m * (fy_old[i] + it->getFy());
        double vz = it->getVz() + 0.5 * dt / m * (fz_old[i] + it->getFz());
        it->setVitesse(Vector(vx, vy, vz));
    }
}

void pas_de_temps(std::vector<Particule>& c, double dt) {
    std::vector<double> fx_old(c.size()), fy_old(c.size()), fz_old(c.size());
    std::size_t i = 0;
    for (auto it = c.begin(); it != c.end(); ++it, ++i) {
        fx_old[i] = it->getFx();
        fy_old[i] = it->getFy();
        fz_old[i] = it->getFz();
    }
    avancer_positions(c, dt, fx_old, fy_old, fz_old);
    recalculer_forces(c);
    avancer_vitesses(c, dt, fx_old, fy_old, fz_old);
}

// std::list<Particule>

void recalculer_forces(std::list<Particule>& c) {
    for (auto& p : c) p.setForce(Vector(0, 0, 0));

    for (auto it1 = c.begin(); it1 != c.end(); ++it1) {
        auto it2 = it1;
        ++it2;
        for (; it2 != c.end(); ++it2) {
            Vector d = it2->getPosition() - it1->getPosition();
            double F_ij = gravitational_force_ij(it1->getMasse(), it2->getMasse(), d);
            it1->setForce(it1->getForce() + d * F_ij);
            it2->setForce(it2->getForce() - d * F_ij);
        }
    }
}

void avancer_positions(std::list<Particule>& c, double dt,
                        const std::vector<double>& fx_old,
                        const std::vector<double>& fy_old,
                        const std::vector<double>& fz_old) {
    std::size_t i = 0;
    for (auto it = c.begin(); it != c.end(); ++it, ++i) {
        double m = it->getMasse();
        double x = it->getX() + dt * it->getVx() + 0.5 * dt * dt * fx_old[i] / m;
        double y = it->getY() + dt * it->getVy() + 0.5 * dt * dt * fy_old[i] / m;
        double z = it->getZ() + dt * it->getVz() + 0.5 * dt * dt * fz_old[i] / m;
        it->setPosition(Vector(x, y, z));
    }
}

void avancer_vitesses(std::list<Particule>& c, double dt,
                       const std::vector<double>& fx_old,
                       const std::vector<double>& fy_old,
                       const std::vector<double>& fz_old) {
    std::size_t i = 0;
    for (auto it = c.begin(); it != c.end(); ++it, ++i) {
        double m = it->getMasse();
        double vx = it->getVx() + 0.5 * dt / m * (fx_old[i] + it->getFx());
        double vy = it->getVy() + 0.5 * dt / m * (fy_old[i] + it->getFy());
        double vz = it->getVz() + 0.5 * dt / m * (fz_old[i] + it->getFz());
        it->setVitesse(Vector(vx, vy, vz));
    }
}

void pas_de_temps(std::list<Particule>& c, double dt) {
    std::vector<double> fx_old(c.size()), fy_old(c.size()), fz_old(c.size());
    std::size_t i = 0;
    for (auto it = c.begin(); it != c.end(); ++it, ++i) {
        fx_old[i] = it->getFx();
        fy_old[i] = it->getFy();
        fz_old[i] = it->getFz();
    }
    avancer_positions(c, dt, fx_old, fy_old, fz_old);
    recalculer_forces(c);
    avancer_vitesses(c, dt, fx_old, fy_old, fz_old);
}

// std::deque<Particule>

void recalculer_forces(std::deque<Particule>& c) {
    for (auto& p : c) p.setForce(Vector(0, 0, 0));

    for (auto it1 = c.begin(); it1 != c.end(); ++it1) {
        auto it2 = it1;
        ++it2;
        for (; it2 != c.end(); ++it2) {
            Vector d = it2->getPosition() - it1->getPosition();
            double F_ij = gravitational_force_ij(it1->getMasse(), it2->getMasse(), d);
            it1->setForce(it1->getForce() + d * F_ij);
            it2->setForce(it2->getForce() - d * F_ij);
        }
    }
}

void avancer_positions(std::deque<Particule>& c, double dt,
                        const std::vector<double>& fx_old,
                        const std::vector<double>& fy_old,
                        const std::vector<double>& fz_old) {
    std::size_t i = 0;
    for (auto it = c.begin(); it != c.end(); ++it, ++i) {
        double m = it->getMasse();
        double x = it->getX() + dt * it->getVx() + 0.5 * dt * dt * fx_old[i] / m;
        double y = it->getY() + dt * it->getVy() + 0.5 * dt * dt * fy_old[i] / m;
        double z = it->getZ() + dt * it->getVz() + 0.5 * dt * dt * fz_old[i] / m;
        it->setPosition(Vector(x, y, z));
    }
}

void avancer_vitesses(std::deque<Particule>& c, double dt,
                       const std::vector<double>& fx_old,
                       const std::vector<double>& fy_old,
                       const std::vector<double>& fz_old) {
    std::size_t i = 0;
    for (auto it = c.begin(); it != c.end(); ++it, ++i) {
        double m = it->getMasse();
        double vx = it->getVx() + 0.5 * dt / m * (fx_old[i] + it->getFx());
        double vy = it->getVy() + 0.5 * dt / m * (fy_old[i] + it->getFy());
        double vz = it->getVz() + 0.5 * dt / m * (fz_old[i] + it->getFz());
        it->setVitesse(Vector(vx, vy, vz));
    }
}

void pas_de_temps(std::deque<Particule>& c, double dt) {
    std::vector<double> fx_old(c.size()), fy_old(c.size()), fz_old(c.size());
    std::size_t i = 0;
    for (auto it = c.begin(); it != c.end(); ++it, ++i) {
        fx_old[i] = it->getFx();
        fy_old[i] = it->getFy();
        fz_old[i] = it->getFz();
    }
    avancer_positions(c, dt, fx_old, fy_old, fz_old);
    recalculer_forces(c);
    avancer_vitesses(c, dt, fx_old, fy_old, fz_old);
}

#endif