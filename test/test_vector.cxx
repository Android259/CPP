// Question 2 : tests de la classe Vector<Dim>.
// Un test par constructeur / operateur / methode, instancie en Dim = 3.

#include "Vector.hpp"
#include <gtest/gtest.h>
#include <sstream>

using Vec3 = Vector<3>;

TEST(VectorTest, ConstructeurParDefaut) {
    Vec3 v;
    EXPECT_EQ(v, Vec3({0.0, 0.0, 0.0}));
}

TEST(VectorTest, ConstructeurAvecCoordonnees) {
    Vec3 v{1.0, 2.0, 3.0};
    EXPECT_DOUBLE_EQ(v.getX(), 1.0);
    EXPECT_DOUBLE_EQ(v.getY(), 2.0);
    EXPECT_DOUBLE_EQ(v.getZ(), 3.0);
}

TEST(VectorTest, ConstructeurParCopie) {
    Vec3 a{1.0, 2.0, 3.0};
    Vec3 b(a);
    EXPECT_EQ(b, a);
}

TEST(VectorTest, ConstructeurParDeplacement) {
    Vec3 a{1.0, 2.0, 3.0};
    Vec3 b(std::move(a));
    EXPECT_EQ(b, Vec3({1.0, 2.0, 3.0}));
}

TEST(VectorTest, OperateurAffectationParCopie) {
    Vec3 a{1.0, 2.0, 3.0};
    Vec3 b;
    b = a;
    EXPECT_EQ(b, a);
}

TEST(VectorTest, OperateurAffectationParDeplacement) {
    Vec3 a{1.0, 2.0, 3.0};
    Vec3 b;
    b = std::move(a);
    EXPECT_EQ(b, Vec3({1.0, 2.0, 3.0}));
}

TEST(VectorTest, GetXGetYGetZ) {
    Vec3 v{1.0, 2.0, 3.0};
    EXPECT_DOUBLE_EQ(v.getX(), 1.0);
    EXPECT_DOUBLE_EQ(v.getY(), 2.0);
    EXPECT_DOUBLE_EQ(v.getZ(), 3.0);
}

TEST(VectorTest, OperateurIndexation) {
    Vec3 v{1.0, 2.0, 3.0};
    EXPECT_DOUBLE_EQ(v[0], 1.0);
    EXPECT_DOUBLE_EQ(v[1], 2.0);
    EXPECT_DOUBLE_EQ(v[2], 3.0);
}

TEST(VectorTest, OperateurPlusEgal) {
    Vec3 v{1.0, 1.0, 1.0};
    v += Vec3({1.0, 2.0, 3.0});
    EXPECT_EQ(v, Vec3({2.0, 3.0, 4.0}));
}

TEST(VectorTest, OperateurMoinsEgal) {
    Vec3 v{2.0, 3.0, 4.0};
    v -= Vec3({1.0, 1.0, 1.0});
    EXPECT_EQ(v, Vec3({1.0, 2.0, 3.0}));
}

TEST(VectorTest, OperateurMultiplieEgal) {
    Vec3 v{1.0, 2.0, 3.0};
    v *= 2.0;
    EXPECT_EQ(v, Vec3({2.0, 4.0, 6.0}));
}

TEST(VectorTest, OperateurDiviseEgal) {
    Vec3 v{2.0, 4.0, 6.0};
    v /= 2.0;
    EXPECT_EQ(v, Vec3({1.0, 2.0, 3.0}));
}

TEST(VectorTest, OperateurOppose) {
    Vec3 v{1.0, -2.0, 3.0};
    EXPECT_EQ(-v, Vec3({-1.0, 2.0, -3.0}));
}

TEST(VectorTest, Norm) {
    Vec3 v{3.0, 4.0, 0.0};
    EXPECT_DOUBLE_EQ(v.norm(), 5.0);
}

TEST(VectorTest, OperateurPlus) {
    Vec3 a{1.0, 2.0, 3.0};
    Vec3 b{4.0, 5.0, 6.0};
    EXPECT_EQ(a + b, Vec3({5.0, 7.0, 9.0}));
}

TEST(VectorTest, OperateurMoins) {
    Vec3 a{4.0, 5.0, 6.0};
    Vec3 b{1.0, 2.0, 3.0};
    EXPECT_EQ(a - b, Vec3({3.0, 3.0, 3.0}));
}

TEST(VectorTest, OperateurMultiplieParScalaireADroite) {
    Vec3 v{1.0, -2.0, 3.0};
    EXPECT_EQ(v*2.0, Vec3({2.0, -4.0, 6.0}));
}

TEST(VectorTest, OperateurMultiplieParScalaireAGauche) {
    Vec3 v{1.0, -2.0, 3.0};
    EXPECT_EQ(2.0*v, Vec3({2.0, -4.0, 6.0}));
}

TEST(VectorTest, OperateurDivise) {
    Vec3 v{2.0, -4.0, 6.0};
    EXPECT_EQ(v / 2.0, Vec3({1.0, -2.0, 3.0}));
}

TEST(VectorTest, OperateurEgalite) {
    EXPECT_TRUE(Vec3({1.0, 2.0, 3.0}) == Vec3({1.0, 2.0, 3.0}));
    EXPECT_FALSE(Vec3({1.0, 2.0, 3.0}) == Vec3({1.0, 2.0, 4.0}));
}

TEST(VectorTest, OperateurDifference) {
    EXPECT_TRUE(Vec3({1.0, 2.0, 3.0}) != Vec3({1.0, 2.0, 4.0}));
    EXPECT_FALSE(Vec3({1.0, 2.0, 3.0}) != Vec3({1.0, 2.0, 3.0}));
}

TEST(VectorTest, OperateurMultiplieProduitScalaire) {
    Vec3 a{1.0, 2.0, 3.0};
    Vec3 b{4.0, 5.0, 6.0};
    EXPECT_DOUBLE_EQ(a*b, 1.0*4.0 + 2.0*5.0 + 3.0*6.0);
}

TEST(VectorTest, OperateurFluxSortant) {
    std::ostringstream flux;
    flux << Vec3({1.0, 2.0, 3.0});
    EXPECT_EQ(flux.str(), "(1, 2, 3)");
}

TEST(VectorTest, OperateurFluxEntrant) {
    std::istringstream flux("1.5 2.5 3.5");
    Vec3 v;
    flux >> v;
    EXPECT_EQ(v, Vec3({1.5, 2.5, 3.5}));
}

TEST(VectorTest, DimensionArbitraire) {
    // Vector<Dim> fonctionne pour un Dim quelconque, pas seulement 3
    // (contrairement a l'ancienne version, toujours (x, y, z)).
    Vector<1> v1{4.0};
    EXPECT_DOUBLE_EQ(v1.norm(), 4.0);

    Vector<2> v2{3.0, 4.0};
    EXPECT_DOUBLE_EQ(v2.norm(), 5.0);
    EXPECT_EQ(v2 + Vector<2>({1.0, 1.0}), Vector<2>({4.0, 5.0}));
}
