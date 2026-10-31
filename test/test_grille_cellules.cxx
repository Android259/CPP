// Lab 5, Question 2 : tests de la classe GrilleCellules<Dim> (Lab 4, Questions 3 et 5).
// Un test par methode publique.

#include "GrilleCellules.hpp"
#include <gtest/gtest.h>
#include <algorithm>

template <std::size_t Dim>
static std::vector<Particule<Dim>> particulesAux(const std::vector<Vector<Dim>>& positions) {
    std::vector<Particule<Dim>> particules;
    particules.reserve(positions.size());
    for (std::size_t i = 0; i < positions.size(); ++i) {
        Particule<Dim> p(1.0);
        p.setPosition(positions[i]);
        p.setId(static_cast<int>(i));
        particules.push_back(p);
    }
    return particules;
}

TEST(GrilleCellulesTest, ConstructeurDecoupeSelonRayonCoupure) {
    // nc_d = floor(L_d / rcoup) : 10/2 = 5 cellules sur l'axe 0, 20/2 = 10 sur l'axe 1.
    GrilleCellules<2> grille(std::array<double, 2>{10.0, 20.0}, 2.0);
    EXPECT_EQ(grille.getNombreCellules(0), 5);
    EXPECT_EQ(grille.getNombreCellules(1), 10);
    EXPECT_EQ(grille.getCellules().size(), 50u);
}

TEST(GrilleCellulesTest, ConstructeurGardeAuMoinsUneCellule) {
    // Boite plus petite que rcoup : floor(1/2) = 0, ramene a 1 cellule.
    GrilleCellules<1> grille(std::array<double, 1>{1.0}, 2.0);
    EXPECT_EQ(grille.getNombreCellules(0), 1);
    EXPECT_EQ(grille.getCellules().size(), 1u);
}

TEST(GrilleCellulesTest, PlacerParticules) {
    // Grille 1D de 5 cellules de taille 2 : x=0.5 -> cellule 0, x=8.5 -> cellule 4.
    GrilleCellules<1> grille(std::array<double, 1>{10.0}, 2.0);
    auto particules = particulesAux<1>({Vector<1>({0.5}), Vector<1>({8.5})});
    grille.placerParticules(particules);

    ASSERT_EQ(grille.getCellule(0).taille(), 1u);
    EXPECT_EQ(grille.getCellule(0).getIndicesParticules()[0], 0u);
    ASSERT_EQ(grille.getCellule(4).taille(), 1u);
    EXPECT_EQ(grille.getCellule(4).getIndicesParticules()[0], 1u);
}

TEST(GrilleCellulesTest, MettreAJourDeplaceLaParticule) {
    GrilleCellules<1> grille(std::array<double, 1>{10.0}, 2.0);
    auto particules = particulesAux<1>({Vector<1>({0.5})});
    grille.placerParticules(particules);

    particules[0].setPosition(Vector<1>({8.5})); // passe de la cellule 0 a la cellule 4
    grille.mettreAJour(particules);

    EXPECT_EQ(grille.getCellule(0).taille(), 0u);
    ASSERT_EQ(grille.getCellule(4).taille(), 1u);
    EXPECT_EQ(grille.getCellule(4).getIndicesParticules()[0], 0u);
}

TEST(GrilleCellulesTest, MettreAJourNeFaitRienSiMemeCellule) {
    GrilleCellules<1> grille(std::array<double, 1>{10.0}, 2.0);
    auto particules = particulesAux<1>({Vector<1>({0.5})});
    grille.placerParticules(particules);

    particules[0].setPosition(Vector<1>({1.2})); // toujours dans la cellule 0
    grille.mettreAJour(particules);

    ASSERT_EQ(grille.getCellule(0).taille(), 1u);
    EXPECT_EQ(grille.getCellule(0).getIndicesParticules()[0], 0u);
}

TEST(GrilleCellulesTest, CellulesVoisinesAuCoinEtALInterieur) {
    // Grille 2D 4x4 : une cellule de coin a 4 voisines (elle incluse),
    // une cellule interieure en a 9 (voisinage complet 3x3).
    GrilleCellules<2> grille(std::array<double, 2>{4.0, 4.0}, 1.0);
    EXPECT_EQ(grille.cellulesVoisines(Vector<2>({0.5, 0.5})).size(), 4u);
    EXPECT_EQ(grille.cellulesVoisines(Vector<2>({1.5, 1.5})).size(), 9u);
}

TEST(GrilleCellulesTest, VoisinsDeNeGardeQueLesIndicesSuperieurs) {
    // Le voisinage precalcule est filtre a jCell >= iCell pour que chaque
    // paire de cellules ne soit traitee qu'une fois dans le calcul de force.
    GrilleCellules<2> grille(std::array<double, 2>{4.0, 4.0}, 1.0);
    for (std::size_t iCell = 0; iCell < grille.getCellules().size(); ++iCell) {
        for (std::size_t jCell : grille.voisinsDe(iCell)) {
            EXPECT_GE(jCell, iCell);
        }
    }
}

TEST(GrilleCellulesTest, VoisinsDeCouvreToutesLesPairesVoisines) {
    // Reciproque du test precedent : l'union des listes filtrees doit
    // contenir chaque paire voisine exactement une fois.
    GrilleCellules<1> grille(std::array<double, 1>{4.0}, 1.0); // 4 cellules alignees
    std::size_t nombrePaires = 0;
    for (std::size_t iCell = 0; iCell < grille.getCellules().size(); ++iCell) {
        nombrePaires += grille.voisinsDe(iCell).size();
    }
    // Paires attendues en 1D : 4 paires (i,i) + 3 paires (i,i+1).
    EXPECT_EQ(nombrePaires, 7u);
}
