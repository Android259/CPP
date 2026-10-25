#ifndef CELLULE_HPP
#define CELLULE_HPP

#include <cstddef>
#include <vector>

// Cellule : une case de la grille tensorielle qui decoupe l'Univers.
// Elle ne stocke pas les particules elles-memes, seulement les indices
// (dans le vector<Particule> de l'Univers) des particules actuellement
// localisees dans cette cellule.
class Cellule {
private:
    std::vector<std::size_t> indicesParticules;

public:
    Cellule() = default;

    void ajouterParticule(std::size_t indice) {
        indicesParticules.push_back(indice);
    }

    void vider() {
        indicesParticules.clear();
    }

    const std::vector<std::size_t>& getIndicesParticules() const {
        return indicesParticules;
    }

    std::size_t taille() const {
        return indicesParticules.size();
    }
};

#endif // CELLULE_HPP
