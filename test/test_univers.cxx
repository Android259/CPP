// Question 2 : tests de la classe Univers.

#include "Univers.hpp"
#include <gtest/gtest.h>
#include <cmath>
#include <fstream>
#include <sstream>
#include <cstdio>

TEST(UniversTest, ConstructeurSimplifie) {
    Univers u(2, 3, 2.0, Vector(1.0, 0.0, 0.0));
    EXPECT_EQ(u.getDimension(), 2);
    EXPECT_EQ(u.getNombreParticules(), 3);
    ASSERT_EQ(u.getParticules().size(), 3u);
    for (std::size_t i = 0; i < 3; ++i) {
        EXPECT_DOUBLE_EQ(u.getParticule(i).getMasse(), 2.0);
        EXPECT_EQ(u.getParticule(i).getVitesse(), Vector(1.0, 0.0, 0.0));
        EXPECT_EQ(u.getParticule(i).getId(), static_cast<int>(i));
    }
}

TEST(UniversTest, ConstructeurCompletMemoriseLesContraintes) {
    Univers u(2, 2, 1.0, Vector(0, 0, 0), std::vector<double>{10.0, 20.0}, 2.5, 5.0, 1.0);
    EXPECT_DOUBLE_EQ(u.getLongueurCaracteristique(0), 10.0);
    EXPECT_DOUBLE_EQ(u.getLongueurCaracteristique(1), 20.0);
    EXPECT_DOUBLE_EQ(u.getRayonCoupure(), 2.5);
    EXPECT_DOUBLE_EQ(u.getEpsilon(), 5.0);
    EXPECT_DOUBLE_EQ(u.getSigma(), 1.0);
}

TEST(UniversTest, SetPosition) {
    Univers u(1, 2, 1.0, Vector(0, 0, 0));
    u.setPosition(0, Vector(1.0, 2.0, 3.0));
    EXPECT_EQ(u.getParticule(0).getPosition(), Vector(1.0, 2.0, 3.0));
}

TEST(UniversTest, RecalculerForcesGravitationnelles) {
    // Deux particules de masse 1 separees d'une unite : Fij = m1*m2/r^2 = 1.
    Univers u(1, 2, 1.0, Vector(0, 0, 0));
    u.setPosition(0, Vector(0.0, 0.0, 0.0));
    u.setPosition(1, Vector(1.0, 0.0, 0.0));
    u.recalculerForces();

    EXPECT_DOUBLE_EQ(u.getParticule(0).getForce().getX(), 1.0);
    EXPECT_DOUBLE_EQ(u.getParticule(1).getForce().getX(), -1.0);
}

TEST(UniversTest, RecalculerForcesSommeNulleATroisCorps) {
    // 3e loi de Newton : quelle que soit la configuration, la somme des
    // forces internes (gravitationnelles) sur l'ensemble du systeme est nulle.
    Univers u(2, 3, 1.0, Vector(0, 0, 0));
    u.setPosition(0, Vector(0.0, 0.0, 0.0));
    u.setPosition(1, Vector(1.0, 0.0, 0.0));
    u.setPosition(2, Vector(0.0, 1.0, 0.0));
    u.recalculerForces();

    Vector somme = u.getParticule(0).getForce()
                + u.getParticule(1).getForce()
                + u.getParticule(2).getForce();
    EXPECT_DOUBLE_EQ(somme.getX(), 0.0);
    EXPECT_DOUBLE_EQ(somme.getY(), 0.0);
    EXPECT_DOUBLE_EQ(somme.getZ(), 0.0);
}

TEST(UniversTest, AvancerConserveLaPositionSansForce) {
    // Une seule particule, aucune force : avance en ligne droite (v constante).
    Univers u(1, 1, 1.0, Vector(2.0, 0.0, 0.0));
    u.avancer(0.5);
    EXPECT_DOUBLE_EQ(u.getParticule(0).getPosition().getX(), 1.0);
    EXPECT_DOUBLE_EQ(u.getParticule(0).getVitesse().getX(), 2.0);
}

TEST(UniversTest, LennardJonesForceNulleHorsRayonCoupure) {
    // Au-dela de rcoup, le potentiel de Lennard-Jones est neglige (Question 1) :
    // la force doit etre exactement nulle.
    Univers u(1, 2, 1.0, Vector(0, 0, 0), std::vector<double>{20.0}, 2.5, 1.0, 1.0);
    u.setPosition(0, Vector(0.0, 0.0, 0.0));
    u.setPosition(1, Vector(5.0, 0.0, 0.0)); // separation = 5 > rcoup = 2.5
    u.initialiserCellules();
    u.recalculerForcesLennardJones();

    EXPECT_EQ(u.getParticule(0).getForce(), Vector(0, 0, 0));
    EXPECT_EQ(u.getParticule(1).getForce(), Vector(0, 0, 0));
}

TEST(UniversTest, LennardJonesRepulsifEnDecaDeLEquilibre) {
    // r < 2^(1/6)*sigma : la force doit ecarter les deux particules.
    const double sigma = 1.0;
    const double rEquilibre = std::pow(2.0, 1.0 / 6.0)*sigma;
    Univers u(1, 2, 1.0, Vector(0, 0, 0), std::vector<double>{20.0}, 5.0, 1.0, sigma);
    u.setPosition(0, Vector(0.0, 0.0, 0.0));
    u.setPosition(1, Vector(0.9*rEquilibre, 0.0, 0.0));
    u.initialiserCellules();
    u.recalculerForcesLennardJones();

    EXPECT_LT(u.getParticule(0).getForce().getX(), 0.0); // repousse vers les x negatifs
    EXPECT_GT(u.getParticule(1).getForce().getX(), 0.0); // repousse vers les x positifs
}

TEST(UniversTest, LennardJonesAttractifEntreEquilibreEtCoupure) {
    // 2^(1/6)*sigma < r < rcoup : la force doit rapprocher les deux particules.
    const double sigma = 1.0;
    const double rEquilibre = std::pow(2.0, 1.0 / 6.0)*sigma;
    Univers u(1, 2, 1.0, Vector(0, 0, 0), std::vector<double>{20.0}, 5.0, 1.0, sigma);
    u.setPosition(0, Vector(0.0, 0.0, 0.0));
    u.setPosition(1, Vector(1.5*rEquilibre, 0.0, 0.0));
    u.initialiserCellules();
    u.recalculerForcesLennardJones();

    EXPECT_GT(u.getParticule(0).getForce().getX(), 0.0); // attire vers les x positifs
    EXPECT_LT(u.getParticule(1).getForce().getX(), 0.0); // attire vers les x negatifs
}

TEST(UniversTest, SauvegarderVTKEcritLesDonnees) {
    // Question 3 (Visualisation) : le fichier VTK ecrit doit contenir le bon
    // nombre de points et les positions exactes des particules.
    Univers u(1, 2, 2.0, Vector(0, 0, 0));
    u.setPosition(0, Vector(1.0, 2.0, 3.0));
    u.setPosition(1, Vector(4.0, 5.0, 6.0));

    const std::string nomFichier = "test_sauvegarde.vtu";
    u.sauvegarderVTK(nomFichier);

    std::ifstream fichier(nomFichier);
    ASSERT_TRUE(fichier.is_open());
    std::string contenu((std::istreambuf_iterator<char>(fichier)), std::istreambuf_iterator<char>());

    EXPECT_NE(contenu.find("<VTKFile"), std::string::npos);
    EXPECT_NE(contenu.find("NumberOfPoints=\"2\""), std::string::npos);
    EXPECT_NE(contenu.find("Name=\"Velocity\""), std::string::npos);
    EXPECT_NE(contenu.find("Name=\"Masse\""), std::string::npos);

    // Les positions ecrites doivent pouvoir etre relues telles quelles.
    std::size_t debutBalise = contenu.find("format=\"ascii\">", contenu.find("Position"));
    ASSERT_NE(debutBalise, std::string::npos);
    std::istringstream valeurs(contenu.substr(debutBalise + std::string("format=\"ascii\">").size()));
    double x1, y1, z1, x2, y2, z2;
    valeurs >> x1 >> y1 >> z1 >> x2 >> y2 >> z2;
    EXPECT_DOUBLE_EQ(x1, 1.0);
    EXPECT_DOUBLE_EQ(y1, 2.0);
    EXPECT_DOUBLE_EQ(z1, 3.0);
    EXPECT_DOUBLE_EQ(x2, 4.0);
    EXPECT_DOUBLE_EQ(y2, 5.0);
    EXPECT_DOUBLE_EQ(z2, 6.0);

    std::remove(nomFichier.c_str());
}

TEST(UniversTest, AvancerLennardJonesRepousseDeuxParticulesProches) {
    // Deux particules trop proches (r < sigma) : le potentiel de Lennard-Jones
    // est repulsif, elles doivent s'eloigner l'une de l'autre.
    Univers u(1, 2, 1.0, Vector(0, 0, 0), std::vector<double>{10.0}, 2.5, 1.0, 1.0);
    u.setPosition(0, Vector(4.0, 0.0, 0.0));
    u.setPosition(1, Vector(4.9, 0.0, 0.0));
    u.initialiserCellules();
    u.recalculerForcesLennardJones();

    double distanceInitiale = u.getParticule(1).getPosition().getX() - u.getParticule(0).getPosition().getX();

    for (int pas = 0; pas < 10; ++pas) {
        u.avancerLennardJones(0.0001);
    }

    double distanceFinale = u.getParticule(1).getPosition().getX() - u.getParticule(0).getPosition().getX();
    EXPECT_GT(distanceFinale, distanceInitiale);
}
