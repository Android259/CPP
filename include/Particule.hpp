#ifndef PARTICULE_HPP
#define PARTICULE_HPP

#include "Vector.hpp"

template <std::size_t Dim>
class Particule {
private:
    Vector<Dim> position;
    Vector<Dim> vitesse;
    double masse;
    double invMasse; // = 1.0 / masse, mise en cache pour eviter de diviser par masse a chaque pas de temps
    int id;
    int type;
    Vector<Dim> force;

public:
    // Constructeur a partir de la seule masse : position/vitesse/force nulles.
    Particule(double masse)
        : position(), vitesse(), masse(masse), invMasse(1.0 / masse),
          id(0), type(0), force() {}

    // Constructeur complet.
    Particule(double masse, const Vector<Dim>& position, const Vector<Dim>& vitesse,
              const Vector<Dim>& force, int id, int type)
        : position(position), vitesse(vitesse), masse(masse), invMasse(1.0 / masse),
          id(id), type(type), force(force) {}

    double getMasse() const { return masse; }
    void setMasse(double m) { masse = m; invMasse = 1.0 / m; }

    // 1.0 / masse, deja calcule : a utiliser (multiplier) a la place d'une
    // division par masse dans les boucles de simulation.
    double getInvMasse() const { return invMasse; }

    int getId() const { return id; }
    void setId(int i) { id = i; }

    // Ordre sur les particules : par identifiant. Permet de ranger des
    // particules dans un std::set, qui exige un ordre sur ses elements.
    bool operator<(const Particule& autre) const { return id < autre.id; }

    int getType() const { return type; }
    void setType(int t) { type = t; }

    // Accesseurs vectoriels
    const Vector<Dim>& getPosition() const { return position; }
    void setPosition(const Vector<Dim>& p) { position = p; }

    const Vector<Dim>& getVitesse() const { return vitesse; }
    void setVitesse(const Vector<Dim>& v) { vitesse = v; }

    const Vector<Dim>& getForce() const { return force; }
    void setForce(const Vector<Dim>& f) { force = f; }

    // Ajoute f a la force courante, sans passer par getForce()+setForce()
    // (evite de construire une copie intermediaire du vecteur force).
    void ajouterForce(const Vector<Dim>& f) { force += f; }

    // Accesseurs par composante, conserves pour compatibilite (valables pour Dim >= 1/2/3)
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
