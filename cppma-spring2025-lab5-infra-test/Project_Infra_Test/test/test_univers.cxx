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

TEST(UniversTest, ConstructeurRejetteDimensionInvalide) {
    EXPECT_THROW(Univers(4, 1, 1.0, Vector(0, 0, 0), std::vector<double>{1.0}, 1.0, 1.0, 1.0),
                 std::invalid_argument);
    EXPECT_THROW(Univers(0, 1, 1.0, Vector(0, 0, 0), std::vector<double>{1.0}, 1.0, 1.0, 1.0),
                 std::invalid_argument);
}

TEST(UniversTest, ConstructeurRejetteTailleDeLIncoherente) {
    EXPECT_THROW(Univers(2, 1, 1.0, Vector(0, 0, 0), std::vector<double>{1.0}, 1.0, 1.0, 1.0),
                 std::invalid_argument);
}

TEST(UniversTest, ConstructeurRejetteRayonCoupureNegatifOuNul) {
    EXPECT_THROW(Univers(1, 1, 1.0, Vector(0, 0, 0), std::vector<double>{1.0}, 0.0, 1.0, 1.0),
                 std::invalid_argument);
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

    EXPECT_NEAR(u.getParticule(0).getForce().getX(), 1.0, 1e-9);
    EXPECT_NEAR(u.getParticule(1).getForce().getX(), -1.0, 1e-9);
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
    EXPECT_NEAR(somme.getX(), 0.0, 1e-9);
    EXPECT_NEAR(somme.getY(), 0.0, 1e-9);
    EXPECT_NEAR(somme.getZ(), 0.0, 1e-9);
}

TEST(UniversTest, CelluleDeCoinAQuatreVoisins) {
    // Grille 4x4 (cellules de cote 1) : la cellule de coin (0,0) n'a que 3
    // voisines valides (1,0), (0,1), (1,1) en plus d'elle-meme, soit 4 au total.
    Univers u(2, 1, 1.0, Vector(0, 0, 0), std::vector<double>{4.0, 4.0}, 1.0, 1.0, 1.0);
    u.initialiserCellules();

    auto voisins = u.cellulesVoisines(Vector(0.5, 0.5, 0.0));
    EXPECT_EQ(voisins.size(), 4u);
}

TEST(UniversTest, CelluleInterieureANeufVoisins) {
    // Meme grille 4x4 : la cellule interieure (1,1) a un voisinage complet
    // 3x3 = 9 cellules (decalage -1,0,1 valide sur chaque axe).
    Univers u(2, 1, 1.0, Vector(0, 0, 0), std::vector<double>{4.0, 4.0}, 1.0, 1.0, 1.0);
    u.initialiserCellules();

    auto voisins = u.cellulesVoisines(Vector(1.5, 1.5, 0.0));
    EXPECT_EQ(voisins.size(), 9u);
}

TEST(UniversTest, MettreAJourCellulesDeplaceLaParticule) {
    // Grille 1D de 5 cellules de taille 2 (L=10, rcoup=2).
    Univers u(1, 1, 1.0, Vector(0, 0, 0), std::vector<double>{10.0}, 2.0, 1.0, 1.0);
    u.setPosition(0, Vector(0.5, 0.0, 0.0)); // cellule 0
    u.initialiserCellules();

    ASSERT_EQ(u.getCellule(0).taille(), 1u);
    EXPECT_EQ(u.getCellule(0).getIndicesParticules()[0], 0u);

    u.setPosition(0, Vector(8.5, 0.0, 0.0)); // cellule 4
    u.mettreAJourCellules();

    EXPECT_EQ(u.getCellule(0).taille(), 0u);
    ASSERT_EQ(u.getCellule(4).taille(), 1u);
    EXPECT_EQ(u.getCellule(4).getIndicesParticules()[0], 0u);
}

TEST(UniversTest, MettreAJourCellulesNeFaitRienSiMemeCellule) {
    // Cell-list incrementale : un petit deplacement qui reste dans la meme
    // cellule ne doit pas changer son contenu.
    Univers u(1, 1, 1.0, Vector(0, 0, 0), std::vector<double>{10.0}, 2.0, 1.0, 1.0);
    u.setPosition(0, Vector(0.5, 0.0, 0.0)); // cellule 0
    u.initialiserCellules();

    u.setPosition(0, Vector(0.8, 0.0, 0.0)); // toujours cellule 0
    u.mettreAJourCellules();

    ASSERT_EQ(u.getCellule(0).taille(), 1u);
    EXPECT_EQ(u.getCellule(0).getIndicesParticules()[0], 0u);
}

TEST(UniversTest, AvancerConserveLaPositionSansForce) {
    // Une seule particule, aucune force : avance en ligne droite (v constante).
    Univers u(1, 1, 1.0, Vector(2.0, 0.0, 0.0));
    u.avancer(0.5);
    EXPECT_NEAR(u.getParticule(0).getPosition().getX(), 1.0, 1e-9);
    EXPECT_NEAR(u.getParticule(0).getVitesse().getX(), 2.0, 1e-9);
}

TEST(UniversTest, AvancerConserveLaQuantiteDeMouvement) {
    // Deux masses differentes, uniquement soumises a leur gravite mutuelle :
    // la quantite de mouvement totale (somme m*v) doit rester nulle apres un
    // pas de Stormer-Verlet, par conservation (forces internes seulement).
    Univers u(1, 2, 1.0, Vector(0, 0, 0));
    u.getParticule(0).setMasse(2.0);
    u.getParticule(1).setMasse(3.0);
    u.setPosition(0, Vector(0.0, 0.0, 0.0));
    u.setPosition(1, Vector(2.0, 0.0, 0.0));
    u.recalculerForces();

    u.avancer(0.01);

    double quantiteDeMouvement = u.getParticule(0).getMasse() * u.getParticule(0).getVitesse().getX()
                                + u.getParticule(1).getMasse() * u.getParticule(1).getVitesse().getX();
    EXPECT_NEAR(quantiteDeMouvement, 0.0, 1e-9);
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

TEST(UniversTest, LennardJonesForceNulleAuPointEquilibre) {
    // Le minimum du potentiel de Lennard-Jones est atteint a r = 2^(1/6)*sigma :
    // la force (derivee du potentiel) y est nulle.
    const double sigma = 1.0;
    const double rEquilibre = std::pow(2.0, 1.0 / 6.0) * sigma;
    Univers u(1, 2, 1.0, Vector(0, 0, 0), std::vector<double>{20.0}, 5.0, 1.0, sigma);
    u.setPosition(0, Vector(0.0, 0.0, 0.0));
    u.setPosition(1, Vector(rEquilibre, 0.0, 0.0));
    u.initialiserCellules();
    u.recalculerForcesLennardJones();

    EXPECT_NEAR(u.getParticule(0).getForce().getX(), 0.0, 1e-9);
    EXPECT_NEAR(u.getParticule(1).getForce().getX(), 0.0, 1e-9);
}

TEST(UniversTest, LennardJonesRepulsifEnDecaDeLEquilibre) {
    // r < 2^(1/6)*sigma : la force doit ecarter les deux particules.
    const double sigma = 1.0;
    const double rEquilibre = std::pow(2.0, 1.0 / 6.0) * sigma;
    Univers u(1, 2, 1.0, Vector(0, 0, 0), std::vector<double>{20.0}, 5.0, 1.0, sigma);
    u.setPosition(0, Vector(0.0, 0.0, 0.0));
    u.setPosition(1, Vector(0.9 * rEquilibre, 0.0, 0.0));
    u.initialiserCellules();
    u.recalculerForcesLennardJones();

    EXPECT_LT(u.getParticule(0).getForce().getX(), 0.0); // repousse vers les x negatifs
    EXPECT_GT(u.getParticule(1).getForce().getX(), 0.0); // repousse vers les x positifs
}

TEST(UniversTest, LennardJonesAttractifEntreEquilibreEtCoupure) {
    // 2^(1/6)*sigma < r < rcoup : la force doit rapprocher les deux particules.
    const double sigma = 1.0;
    const double rEquilibre = std::pow(2.0, 1.0 / 6.0) * sigma;
    Univers u(1, 2, 1.0, Vector(0, 0, 0), std::vector<double>{20.0}, 5.0, 1.0, sigma);
    u.setPosition(0, Vector(0.0, 0.0, 0.0));
    u.setPosition(1, Vector(1.5 * rEquilibre, 0.0, 0.0));
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
