#include<set>
#include<list>
#include<deque>
#include<vector>

class Particule {
    private:
    double masse; // attribut caché, inaccessible depuis l'extérieur

    public:
    Particule(double masse); // constructeur

    double getMasse() const; // accesseur (”getter”)
    void setMasse(double m); // mutateur (”setter”)
    };