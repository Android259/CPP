// Question 2 : tests de la classe Cellule.

#include "Cellule.hpp"
#include <gtest/gtest.h>
#include <algorithm>

TEST(CelluleTest, VideALaConstruction) {
    Cellule c;
    EXPECT_EQ(c.taille(), 0u);
    EXPECT_TRUE(c.getIndicesParticules().empty());
}

TEST(CelluleTest, AjouterParticule) {
    Cellule c;
    c.ajouterParticule(3);
    c.ajouterParticule(7);
    EXPECT_EQ(c.taille(), 2u);
    EXPECT_EQ(c.getIndicesParticules()[0], 3u);
    EXPECT_EQ(c.getIndicesParticules()[1], 7u);
}

TEST(CelluleTest, Vider) {
    Cellule c;
    c.ajouterParticule(1);
    c.ajouterParticule(2);
    c.vider();
    EXPECT_EQ(c.taille(), 0u);
    EXPECT_TRUE(c.getIndicesParticules().empty());
}

TEST(CelluleTest, RetirerParticule) {
    // retirerParticule echange avec le dernier element : l'ordre des indices
    // restants n'est pas garanti, seul l'ensemble compte.
    Cellule c;
    c.ajouterParticule(1);
    c.ajouterParticule(2);
    c.ajouterParticule(3);
    c.retirerParticule(2);

    const auto& indices = c.getIndicesParticules();
    EXPECT_EQ(c.taille(), 2u);
    EXPECT_EQ(std::count(indices.begin(), indices.end(), 1u), 1);
    EXPECT_EQ(std::count(indices.begin(), indices.end(), 2u), 0);
    EXPECT_EQ(std::count(indices.begin(), indices.end(), 3u), 1);
}

TEST(CelluleTest, RetirerParticuleAbsenteNeFaitRien) {
    Cellule c;
    c.ajouterParticule(1);
    c.retirerParticule(42); // absente de la cellule : ne doit rien changer
    EXPECT_EQ(c.taille(), 1u);
    EXPECT_EQ(c.getIndicesParticules()[0], 1u);
}
