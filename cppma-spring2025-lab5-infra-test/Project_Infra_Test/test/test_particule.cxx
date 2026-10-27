// Question 2 : tests de la classe Particule.
// Un test par constructeur / methode (ou paire accesseur-mutateur).

#include "Particule.hpp"
#include <gtest/gtest.h>

TEST(ParticuleTest, ConstructeurSimple) {
    Particule p(2.0);
    EXPECT_DOUBLE_EQ(p.getMasse(), 2.0);
    EXPECT_DOUBLE_EQ(p.getInvMasse(), 0.5);
    EXPECT_EQ(p.getPosition(), Vector(0, 0, 0));
    EXPECT_EQ(p.getVitesse(), Vector(0, 0, 0));
    EXPECT_EQ(p.getForce(), Vector(0, 0, 0));
    EXPECT_EQ(p.getId(), 0);
    EXPECT_EQ(p.getType(), 0);
}

TEST(ParticuleTest, ConstructeurComplet) {
    Particule p(1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 42, 1, 8.0, 9.0, 10.0);
    EXPECT_EQ(p.getPosition(), Vector(1.0, 2.0, 3.0));
    EXPECT_EQ(p.getVitesse(), Vector(4.0, 5.0, 6.0));
    EXPECT_DOUBLE_EQ(p.getMasse(), 7.0);
    EXPECT_EQ(p.getId(), 42);
    EXPECT_EQ(p.getType(), 1);
    EXPECT_EQ(p.getForce(), Vector(8.0, 9.0, 10.0));
}

TEST(ParticuleTest, GetMasse) {
    Particule p(3.5);
    EXPECT_DOUBLE_EQ(p.getMasse(), 3.5);
}

TEST(ParticuleTest, SetMasse) {
    Particule p(1.0);
    p.setMasse(4.0);
    EXPECT_DOUBLE_EQ(p.getMasse(), 4.0);
}

TEST(ParticuleTest, GetInvMasse) {
    Particule p(4.0);
    EXPECT_DOUBLE_EQ(p.getInvMasse(), 0.25);
}

TEST(ParticuleTest, SetMasseMetAJourInvMasse) {
    // getInvMasse() doit rester coherent (1/masse) apres un changement de masse.
    Particule p(1.0);
    p.setMasse(4.0);
    EXPECT_DOUBLE_EQ(p.getInvMasse(), 0.25);
}

TEST(ParticuleTest, GetId) {
    Particule p(1.0, 2.0, 3.0, 0, 0, 0, 1.0, 7, 0, 0, 0, 0);
    EXPECT_EQ(p.getId(), 7);
}

TEST(ParticuleTest, SetId) {
    Particule p(1.0);
    p.setId(5);
    EXPECT_EQ(p.getId(), 5);
}

TEST(ParticuleTest, GetType) {
    Particule p(1.0, 2.0, 3.0, 0, 0, 0, 1.0, 0, 9, 0, 0, 0);
    EXPECT_EQ(p.getType(), 9);
}

TEST(ParticuleTest, SetType) {
    Particule p(1.0);
    p.setType(2);
    EXPECT_EQ(p.getType(), 2);
}

TEST(ParticuleTest, GetPosition) {
    Particule p(1.0, 2.0, 3.0, 0, 0, 0, 1.0, 0, 0, 0, 0, 0);
    EXPECT_EQ(p.getPosition(), Vector(1.0, 2.0, 3.0));
}

TEST(ParticuleTest, SetPosition) {
    Particule p(1.0);
    p.setPosition(Vector(1.0, 2.0, 3.0));
    EXPECT_EQ(p.getPosition(), Vector(1.0, 2.0, 3.0));
}

TEST(ParticuleTest, GetVitesse) {
    Particule p(0, 0, 0, 4.0, 5.0, 6.0, 1.0, 0, 0, 0, 0, 0);
    EXPECT_EQ(p.getVitesse(), Vector(4.0, 5.0, 6.0));
}

TEST(ParticuleTest, SetVitesse) {
    Particule p(1.0);
    p.setVitesse(Vector(4.0, 5.0, 6.0));
    EXPECT_EQ(p.getVitesse(), Vector(4.0, 5.0, 6.0));
}

TEST(ParticuleTest, GetForce) {
    Particule p(0, 0, 0, 0, 0, 0, 1.0, 0, 0, 7.0, 8.0, 9.0);
    EXPECT_EQ(p.getForce(), Vector(7.0, 8.0, 9.0));
}

TEST(ParticuleTest, SetForce) {
    Particule p(1.0);
    p.setForce(Vector(7.0, 8.0, 9.0));
    EXPECT_EQ(p.getForce(), Vector(7.0, 8.0, 9.0));
}

TEST(ParticuleTest, AjouterForce) {
    Particule p(1.0);
    p.setForce(Vector(1.0, 1.0, 1.0));
    p.ajouterForce(Vector(2.0, 3.0, 4.0));
    EXPECT_EQ(p.getForce(), Vector(3.0, 4.0, 5.0));
}

TEST(ParticuleTest, GetXGetYGetZ) {
    // Accesseurs par composante de la position, conserves pour compatibilite.
    Particule p(1.0);
    p.setPosition(Vector(1.0, 2.0, 3.0));
    EXPECT_DOUBLE_EQ(p.getX(), 1.0);
    EXPECT_DOUBLE_EQ(p.getY(), 2.0);
    EXPECT_DOUBLE_EQ(p.getZ(), 3.0);
}

TEST(ParticuleTest, GetVxGetVyGetVz) {
    // Accesseurs par composante de la vitesse, conserves pour compatibilite.
    Particule p(1.0);
    p.setVitesse(Vector(4.0, 5.0, 6.0));
    EXPECT_DOUBLE_EQ(p.getVx(), 4.0);
    EXPECT_DOUBLE_EQ(p.getVy(), 5.0);
    EXPECT_DOUBLE_EQ(p.getVz(), 6.0);
}

TEST(ParticuleTest, GetFxGetFyGetFz) {
    // Accesseurs par composante de la force, conserves pour compatibilite.
    Particule p(1.0);
    p.setForce(Vector(7.0, 8.0, 9.0));
    EXPECT_DOUBLE_EQ(p.getFx(), 7.0);
    EXPECT_DOUBLE_EQ(p.getFy(), 8.0);
    EXPECT_DOUBLE_EQ(p.getFz(), 9.0);
}
