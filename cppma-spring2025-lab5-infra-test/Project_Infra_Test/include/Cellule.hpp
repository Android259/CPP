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

    // Reserve a l'avance la capacite du vecteur interne, pour eviter des
    // reallocations lors des push_back de ajouterParticule (cf.
    // GrilleCellules::placerParticules, qui estime cette capacite a partir
    // de la densite moyenne de particules par cellule).
    void reserver(std::size_t capacite) {
        indicesParticules.reserve(capacite);
    }

    void vider() {
        indicesParticules.clear();
    }

    // Retire la particule d'indice 'indice' de la cellule (cell-list
    // incrementale : appele uniquement quand une particule change de
    // cellule entre deux pas, cf. Univers::mettreAJourCellules). L'ordre des
    // indices restants n'a pas d'importance pour le calcul des forces, d'ou
    // l'echange avec le dernier element (O(1) une fois l'indice trouve).
    void retirerParticule(std::size_t indice) {
        for (std::size_t k = 0; k < indicesParticules.size(); ++k) {
            if (indicesParticules[k] == indice) {
                indicesParticules[k] = indicesParticules.back();
                indicesParticules.pop_back();
                return;
            }
        }
    }

    const std::vector<std::size_t>& getIndicesParticules() const {
        return indicesParticules;
    }

    std::size_t taille() const {
        return indicesParticules.size();
    }
};

#endif // CELLULE_HPP
