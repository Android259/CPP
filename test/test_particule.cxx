// Question 2 : tests de la classe Particule<Dim>.
// Un test par constructeur / methode (ou paire accesseur-mutateur), instancie en Dim = 3.

#include "Particule.hpp"
#include <gtest/gtest.h>

using Vec3 = Vector<3>;
using Part3 = Particule<3>;

TEST(ParticuleTest, ConstructeurSimple) {
    Part3 p(2.0);
    EXPECT_DOUBLE_EQ(p.getMasse(), 2.0);
    EXPECT_DOUBLE_EQ(p.getInvMasse(), 0.5);
    EXPECT_EQ(p.getPosition(), Vec3());
    EXPECT_EQ(p.getVitesse(), Vec3());
    EXPECT_EQ(p.getForce(), Vec3());
    EXPECT_EQ(p.getId(), 0);
    EXPECT_EQ(p.getType(), 0);
}

TEST(ParticuleTest, ConstructeurComplet) {
    Part3 p(7.0, Vec3({1.0, 2.0, 3.0}), Vec3({4.0, 5.0, 6.0}), Vec3({8.0, 9.0, 10.0}), 42, 1);
    EXPECT_EQ(p.getPosition(), Vec3({1.0, 2.0, 3.0}));
    EXPECT_EQ(p.getVitesse(), Vec3({4.0, 5.0, 6.0}));
    EXPECT_DOUBLE_EQ(p.getMasse(), 7.0);
    EXPECT_EQ(p.getId(), 42);
    EXPECT_EQ(p.getType(), 1);
    EXPECT_EQ(p.getForce(), Vec3({8.0, 9.0, 10.0}));
}

TEST(ParticuleTest, SetMasse) {
    Part3 p(1.0);
    p.setMasse(4.0);
    EXPECT_DOUBLE_EQ(p.getMasse(), 4.0);
}

TEST(ParticuleTest, SetMasseMetAJourInvMasse) {
    // getInvMasse() doit rester coherent (1/masse) apres un changement de masse.
    Part3 p(1.0);
    p.setMasse(4.0);
    EXPECT_DOUBLE_EQ(p.getInvMasse(), 0.25);
}

TEST(ParticuleTest, SetId) {
    Part3 p(1.0);
    p.setId(5);
    EXPECT_EQ(p.getId(), 5);
}

TEST(ParticuleTest, SetType) {
    Part3 p(1.0);
    p.setType(2);
    EXPECT_EQ(p.getType(), 2);
}

TEST(ParticuleTest, SetPosition) {
    Part3 p(1.0);
    p.setPosition(Vec3({1.0, 2.0, 3.0}));
    EXPECT_EQ(p.getPosition(), Vec3({1.0, 2.0, 3.0}));
}

TEST(ParticuleTest, SetVitesse) {
    Part3 p(1.0);
    p.setVitesse(Vec3({4.0, 5.0, 6.0}));
    EXPECT_EQ(p.getVitesse(), Vec3({4.0, 5.0, 6.0}));
}

TEST(ParticuleTest, SetForce) {
    Part3 p(1.0);
    p.setForce(Vec3({7.0, 8.0, 9.0}));
    EXPECT_EQ(p.getForce(), Vec3({7.0, 8.0, 9.0}));
}

TEST(ParticuleTest, AjouterForce) {
    Part3 p(1.0);
    p.setForce(Vec3({1.0, 1.0, 1.0}));
    p.ajouterForce(Vec3({2.0, 3.0, 4.0}));
    EXPECT_EQ(p.getForce(), Vec3({3.0, 4.0, 5.0}));
}

TEST(ParticuleTest, GetXGetYGetZ) {
    // Accesseurs par composante de la position, conserves pour compatibilite.
    Part3 p(1.0);
    p.setPosition(Vec3({1.0, 2.0, 3.0}));
    EXPECT_DOUBLE_EQ(p.getX(), 1.0);
    EXPECT_DOUBLE_EQ(p.getY(), 2.0);
    EXPECT_DOUBLE_EQ(p.getZ(), 3.0);
}

TEST(ParticuleTest, GetVxGetVyGetVz) {
    // Accesseurs par composante de la vitesse, conserves pour compatibilite.
    Part3 p(1.0);
    p.setVitesse(Vec3({4.0, 5.0, 6.0}));
    EXPECT_DOUBLE_EQ(p.getVx(), 4.0);
    EXPECT_DOUBLE_EQ(p.getVy(), 5.0);
    EXPECT_DOUBLE_EQ(p.getVz(), 6.0);
}

TEST(ParticuleTest, GetFxGetFyGetFz) {
    // Accesseurs par composante de la force, conserves pour compatibilite.
    Part3 p(1.0);
    p.setForce(Vec3({7.0, 8.0, 9.0}));
    EXPECT_DOUBLE_EQ(p.getFx(), 7.0);
    EXPECT_DOUBLE_EQ(p.getFy(), 8.0);
    EXPECT_DOUBLE_EQ(p.getFz(), 9.0);
}
