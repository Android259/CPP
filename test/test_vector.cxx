// Question 2 : tests de la classe Vector.
// Un test par constructeur / operateur / methode.

#include "Vector.hpp"
#include <gtest/gtest.h>
#include <sstream>

TEST(VectorTest, ConstructeurParDefaut) {
    Vector v;
    EXPECT_EQ(v, Vector(0.0, 0.0, 0.0));
}

TEST(VectorTest, ConstructeurAvecCoordonnees) {
    Vector v(1.0, 2.0, 3.0);
    EXPECT_DOUBLE_EQ(v.getX(), 1.0);
    EXPECT_DOUBLE_EQ(v.getY(), 2.0);
    EXPECT_DOUBLE_EQ(v.getZ(), 3.0);
}

TEST(VectorTest, ConstructeurParCopie) {
    Vector a(1.0, 2.0, 3.0);
    Vector b(a);
    EXPECT_EQ(b, a);
}

TEST(VectorTest, ConstructeurParDeplacement) {
    Vector a(1.0, 2.0, 3.0);
    Vector b(std::move(a));
    EXPECT_EQ(b, Vector(1.0, 2.0, 3.0));
}

TEST(VectorTest, OperateurAffectationParCopie) {
    Vector a(1.0, 2.0, 3.0);
    Vector b;
    b = a;
    EXPECT_EQ(b, a);
}

TEST(VectorTest, OperateurAffectationParDeplacement) {
    Vector a(1.0, 2.0, 3.0);
    Vector b;
    b = std::move(a);
    EXPECT_EQ(b, Vector(1.0, 2.0, 3.0));
}

TEST(VectorTest, GetXGetYGetZ) {
    Vector v(1.0, 2.0, 3.0);
    EXPECT_DOUBLE_EQ(v.getX(), 1.0);
    EXPECT_DOUBLE_EQ(v.getY(), 2.0);
    EXPECT_DOUBLE_EQ(v.getZ(), 3.0);
}

TEST(VectorTest, OperateurIndexation) {
    Vector v(1.0, 2.0, 3.0);
    EXPECT_DOUBLE_EQ(v[0], 1.0);
    EXPECT_DOUBLE_EQ(v[1], 2.0);
    EXPECT_DOUBLE_EQ(v[2], 3.0);
}

TEST(VectorTest, OperateurPlusEgal) {
    Vector v(1.0, 1.0, 1.0);
    v += Vector(1.0, 2.0, 3.0);
    EXPECT_EQ(v, Vector(2.0, 3.0, 4.0));
}

TEST(VectorTest, OperateurMoinsEgal) {
    Vector v(2.0, 3.0, 4.0);
    v -= Vector(1.0, 1.0, 1.0);
    EXPECT_EQ(v, Vector(1.0, 2.0, 3.0));
}

TEST(VectorTest, OperateurMultiplieEgal) {
    Vector v(1.0, 2.0, 3.0);
    v *= 2.0;
    EXPECT_EQ(v, Vector(2.0, 4.0, 6.0));
}

TEST(VectorTest, OperateurDiviseEgal) {
    Vector v(2.0, 4.0, 6.0);
    v /= 2.0;
    EXPECT_EQ(v, Vector(1.0, 2.0, 3.0));
}

TEST(VectorTest, OperateurOppose) {
    Vector v(1.0, -2.0, 3.0);
    EXPECT_EQ(-v, Vector(-1.0, 2.0, -3.0));
}

TEST(VectorTest, Norm) {
    Vector v(3.0, 4.0, 0.0);
    EXPECT_DOUBLE_EQ(v.norm(), 5.0);
}

TEST(VectorTest, OperateurPlus) {
    Vector a(1.0, 2.0, 3.0);
    Vector b(4.0, 5.0, 6.0);
    EXPECT_EQ(a + b, Vector(5.0, 7.0, 9.0));
}

TEST(VectorTest, OperateurMoins) {
    Vector a(4.0, 5.0, 6.0);
    Vector b(1.0, 2.0, 3.0);
    EXPECT_EQ(a - b, Vector(3.0, 3.0, 3.0));
}

TEST(VectorTest, OperateurMultiplieParScalaireADroite) {
    Vector v(1.0, -2.0, 3.0);
    EXPECT_EQ(v*2.0, Vector(2.0, -4.0, 6.0));
}

TEST(VectorTest, OperateurMultiplieParScalaireAGauche) {
    Vector v(1.0, -2.0, 3.0);
    EXPECT_EQ(2.0*v, Vector(2.0, -4.0, 6.0));
}

TEST(VectorTest, OperateurDivise) {
    Vector v(2.0, -4.0, 6.0);
    EXPECT_EQ(v / 2.0, Vector(1.0, -2.0, 3.0));
}

TEST(VectorTest, OperateurEgalite) {
    EXPECT_TRUE(Vector(1.0, 2.0, 3.0) == Vector(1.0, 2.0, 3.0));
    EXPECT_FALSE(Vector(1.0, 2.0, 3.0) == Vector(1.0, 2.0, 4.0));
}

TEST(VectorTest, OperateurDifference) {
    EXPECT_TRUE(Vector(1.0, 2.0, 3.0) != Vector(1.0, 2.0, 4.0));
    EXPECT_FALSE(Vector(1.0, 2.0, 3.0) != Vector(1.0, 2.0, 3.0));
}

TEST(VectorTest, OperateurMultiplieProduitScalaire) {
    Vector a(1.0, 2.0, 3.0);
    Vector b(4.0, 5.0, 6.0);
    EXPECT_DOUBLE_EQ(a*b, 1.0*4.0 + 2.0*5.0 + 3.0*6.0);
}

TEST(VectorTest, OperateurFluxSortant) {
    std::ostringstream flux;
    flux << Vector(1.0, 2.0, 3.0);
    EXPECT_EQ(flux.str(), "(1, 2, 3)");
}

TEST(VectorTest, OperateurFluxEntrant) {
    std::istringstream flux("1.5 2.5 3.5");
    Vector v;
    flux >> v;
    EXPECT_EQ(v, Vector(1.5, 2.5, 3.5));
}
