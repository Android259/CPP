#include <gtest/gtest.h>

// Test des vecteurs
#include <cmath>
#include "vecteur.hpp"
TEST(VecteurTests, creation){
    // Dimension 
    Vecteur<1> v1 = Vecteur<1>(3.0);
    EXPECT_EQ(v1[0], 3.0) << "Erreur pour l'initialisation d'un vecteur a une dimension avec un scalaire";

    double valeurs[1] = {-2.0};
    Vecteur<1> v2 = Vecteur<1>(valeurs);
    EXPECT_EQ(v2[0], -2.0) << "Erreur pour l'initialisation d'un vecteur a une dimension avec un tableau";

    Vecteur<1> v3 = {-0.0};
    EXPECT_EQ(v3[0], -0.0) << "Erreur pour l'initialisation d'un vecteur a une dimension avec une liste";

    // Dimension 2
    Vecteur<2> v4 = Vecteur<2>(0.12);
    EXPECT_EQ(v4[0], 0.12) << "Erreur pour l'initialisation d'un vecteur a deux dimension avec un scalaire pour la première composante";
    EXPECT_EQ(v4[1], 0.12) << "Erreur pour l'initialisation d'un vecteur a deux dimension avec un scalaire pour la deuxieme composante";

    double valeurs2[2] = {-1.0, 15.0};
    Vecteur<2> v5 = Vecteur<2>(valeurs2);
    EXPECT_EQ(v5[0], -1.0) << "Erreur pour l'initialisation d'un vecteur a deux dimension avec un tableau pour la première composante";
    EXPECT_EQ(v5[1], 15.0) << "Erreur pour l'initialisation d'un vecteur a deux dimension avec un tableau pour la deuxième composante";

    Vecteur<2> v6 = {1.0, 0.0};
    EXPECT_EQ(v6[0], 1.0) << "Erreur pour l'initialisation d'un vecteur a deux dimension avec une liste pour la première composante";
    EXPECT_EQ(v6[1], 0.0) << "Erreur pour l'initialisation d'un vecteur a deux dimension avec une liste pour la deuxième composante";

    // Dimension 3
    Vecteur<3> v7 = Vecteur<3>(0.1);
    EXPECT_EQ(v7[0], 0.1) << "Erreur pour l'initialisation d'un vecteur a trois dimension avec un scalaire pour la première composante";
    EXPECT_EQ(v7[1], 0.1) << "Erreur pour l'initialisation d'un vecteur a trois dimension avec un scalaire pour la deuxieme composante";
    EXPECT_EQ(v7[2], 0.1) << "Erreur pour l'initialisation d'un vecteur a trois dimension avec un scalaire pour la troisième composante";

    double valeurs3[3] = {-1.0, 15.0, 154.0};
    Vecteur<3> v8 = Vecteur<3>(valeurs3);
    EXPECT_EQ(v8[0], -1.0) << "Erreur pour l'initialisation d'un vecteur a trois dimension avec un tableau pour la première composante";
    EXPECT_EQ(v8[1], 15.0) << "Erreur pour l'initialisation d'un vecteur a trois dimension avec un tableau pour la deuxième composante";
    EXPECT_EQ(v8[2], 154.0) << "Erreur pour l'initialisation d'un vecteur a trois dimension avec un tableau pour la troisième composante";

    Vecteur<3> v9 = {1.0, 0.0, 15.0};
    EXPECT_EQ(v9[0], 1.0) << "Erreur pour l'initialisation d'un vecteur a trois dimension avec une liste pour la première composante";
    EXPECT_EQ(v9[1], 0.0) << "Erreur pour l'initialisation d'un vecteur a trois dimension avec une liste pour la deuxième composante";
    EXPECT_EQ(v9[2], 15.0) << "Erreur pour l'initialisation d'un vecteur a trois dimension avec une liste pour la troisième composante";
}

/// @todo tester l'égalité et la non égalité

TEST(VecteurTests, egaliteDifference){
    Vecteur<1> v1 = {0.0};
    Vecteur<1> v2 = {2.0};
    EXPECT_EQ(v1 == v2, false) << "Erreur egalite de deux vecteurs differents de dimension 1";
    EXPECT_EQ(v1 != v2, true) << "Erreur non egalite de deux vecteurs differents de dimension 1";
    Vecteur<1> v3 = {2.0};
    EXPECT_EQ(v2 == v3, true) << "Erreur egalite de deux vecteurs egaux de dimension 1";
    EXPECT_EQ(v2 != v3, false) << "Erreur non egalite de deux vecteurs egaux de dimension 1";

    Vecteur<2> v4 = {1.0, 1.0};
    Vecteur<2> v5 = {3.0, 3.0};
    EXPECT_EQ(v4 == v5, false) << "Erreur egalite de deux vecteurs differents partout de dimension 2";
    EXPECT_EQ(v4 != v5, true) << "Erreur non egalite de deux vecteurs differents partout de dimension 2";
    Vecteur<2> v6 = {3.0, 1.0};
    EXPECT_EQ(v4 == v6, false) << "Erreur egalite de deux vecteurs differents sur la premiere composante de dimension 2";
    EXPECT_EQ(v4 != v6, true) << "Erreur non egalite de deux vecteurs differents sur la premiere composante de dimension 2";
    EXPECT_EQ(v5 == v6, false) << "Erreur egalite de deux vecteurs differents sur la deuxieme composante de dimension 2";
    EXPECT_EQ(v5 != v6, true) << "Erreur non egalite de deux vecteurs differents sur la deuxieme composante de dimension 2";
    Vecteur<2> v7 = {3.0, 3.0};
    EXPECT_EQ(v5 == v7, true) << "Erreur egalite de deux vecteurs egaux de dimension 2";
    EXPECT_EQ(v5 != v7, false) << "Erreur non egalite de deux vecteurs egaux de dimension 2";
    

    Vecteur<3> v8 = {2.0, 2.0, 2.0};
    Vecteur<3> v9 = {4.0, 4.0, 4.0};
    EXPECT_EQ(v8 == v9, false) << "Erreur egalite de deux vecteurs differents partout de dimension 3";
    EXPECT_EQ(v8 != v9, true) << "Erreur non egalite de deux vecteurs differents partout de dimension 3";
    Vecteur<3> v10 = {4.0, 2.0, 2.0};
    Vecteur<3> v11 = {2.0, 4.0, 2.0};
    Vecteur<3> v12 = {2.0, 2.0, 4.0};
    EXPECT_EQ(v8 == v10, false) << "Erreur egalite des deux vecteurs : "<< v8 << " et " << v10;
    EXPECT_EQ(v8 != v10, true) << "Erreur non egalite des deux vecteurs : "<< v8 << " et " << v10;
    EXPECT_EQ(v9 == v10, false) << "Erreur egalite des deux vecteurs : "<< v9 << " et " << v10;
    EXPECT_EQ(v9 != v10, true) << "Erreur non egalite des deux vecteurs : "<< v9 << " et " << v10;
    EXPECT_EQ(v8 == v11, false) << "Erreur egalite des deux vecteurs : "<< v8 << " et " << v11;
    EXPECT_EQ(v8 != v11, true) << "Erreur non egalite des deux vecteurs : "<< v8 << " et " << v11;
    EXPECT_EQ(v9 == v11, false) << "Erreur egalite des deux vecteurs : "<< v9 << " et " << v11;
    EXPECT_EQ(v9 != v11, true) << "Erreur non egalite des deux vecteurs : "<< v9 << " et " << v11;
    EXPECT_EQ(v8 == v12, false) << "Erreur egalite des deux vecteurs : "<< v8 << " et " << v12;
    EXPECT_EQ(v8 != v12, true) << "Erreur non egalite des deux vecteurs : "<< v8 << " et " << v12;
    EXPECT_EQ(v9 == v12, false) << "Erreur egalite des deux vecteurs : "<< v9 << " et " << v12;
    EXPECT_EQ(v9 != v12, true) << "Erreur non egalite des deux vecteurs : "<< v9 << " et " << v12;
    Vecteur<3> v13 = {4.0, 4.0, 4.0};
    EXPECT_EQ(v9 == v13, true) << "Erreur egalite de deux vecteurs egaux de dimension 2";
    EXPECT_EQ(v9 != v13, false) << "Erreur non egalite de deux vecteurs egaux de dimension 2";
}

TEST(VecteurTests, affectation){
    Vecteur<1> v1 = {-2.0};
    Vecteur<1> v2 = v1;
    EXPECT_EQ(v1[0], v2[0]) << "Erreur d'affectation pour un vecteur de dimension 1";

    Vecteur<2> v3 = {-1.0, 1.2};
    Vecteur<2> v4 = v3;
    EXPECT_EQ(v3[0], v4[0]) << "Erreur d'affectation pour un vecteur de dimension 2 pour la premiere composante";
    EXPECT_EQ(v3[1], v4[1]) << "Erreur d'affectation pour un vecteur de dimension 2 pour la seconde composante";

    Vecteur<3> v5 = {-12.0, 1.23, 1.45};
    Vecteur<3> v6 = v5;
    EXPECT_EQ(v5[0], v6[0]) << "Erreur d'affectation pour un vecteur de dimension 3 pour la premiere composante";
    EXPECT_EQ(v5[1], v6[1]) << "Erreur d'affectation pour un vecteur de dimension 3 pour la seconde composante";
    EXPECT_EQ(v5[2], v6[2]) << "Erreur d'affectation pour un vecteur de dimension 3 pour la troisième composante";
}

TEST(VecteurTests, increment){
    Vecteur<1> v1 = {-2.0};
    Vecteur<1> v2 = {56.6};
    Vecteur<1> v3 = v1;
    v3 += v2;
    EXPECT_EQ(v3[0], v1[0] + v2[0]) << "Erreur d'increment pour un vecteur de dimension 1";

    Vecteur<2> v4 = {-1.0, 1.2};
    Vecteur<2> v5 = {90.0, 78.65};
    Vecteur<2> v6 = v4;
    v6 += v5;
    EXPECT_EQ(v6[0], v4[0] + v5[0]) << "Erreur d'increment pour un vecteur de dimension 2 pour la premiere composante";
    EXPECT_EQ(v6[1], v4[1] + v5[1]) << "Erreur d'increment pour un vecteur de dimension 2 pour la seconde composante";

    Vecteur<3> v7 = {-12.0, 1.23, 1.45};
    Vecteur<3> v8 = {84.0, 8744.0, 0.4855};
    Vecteur<3> v9 = v7;
    v9 += v8;
    EXPECT_EQ(v9[0], v7[0] + v8[0]) << "Erreur d'increment pour un vecteur de dimension 3 pour la premiere composante";
    EXPECT_EQ(v9[1], v7[1] + v8[1]) << "Erreur d'increment pour un vecteur de dimension 3 pour la seconde composante";
    EXPECT_EQ(v9[2], v7[2] + v8[2]) << "Erreur d'increment pour un vecteur de dimension 3 pour la troisième composante";
}

TEST(VecteurTests, multiplication){
    Vecteur<1> v1 = {-2.0};
    Vecteur<1> v2 = v1;
    v2 *= 5.265;
    EXPECT_EQ(v2[0], 5.265*v1[0]) << "Erreur de multiplication pour un vecteur de dimension 1";

    Vecteur<2> v3 = {-1.0, 1.2};
    Vecteur<2> v4 = v3;
    v4 *= 15.15;
    EXPECT_EQ(v4[0], 15.15*v3[0]) << "Erreur de multiplication pour un vecteur de dimension 2 pour la premiere composante";
    EXPECT_EQ(v4[1], 15.15*v3[1]) << "Erreur de multiplication pour un vecteur de dimension 2 pour la seconde composante";

    Vecteur<3> v5 = {-12.0, 1.23, 1.45};
    Vecteur<3> v6 = v5;
    v6 *= 5.5;
    EXPECT_EQ(v6[0], 5.5*v5[0]) << "Erreur de multiplication pour un vecteur de dimension 3 pour la premiere composante";
    EXPECT_EQ(v6[1], 5.5*v5[1]) << "Erreur de multiplication pour un vecteur de dimension 3 pour la seconde composante";
    EXPECT_EQ(v6[2], 5.5*v5[2]) << "Erreur de multiplication pour un vecteur de dimension 3 pour la troisième composante";
}

TEST(VecteurTests, decrement){
    Vecteur<1> v1 = {-2.0};
    Vecteur<1> v2 = {56.6};
    Vecteur<1> v3 = v1;
    v3 -= v2;
    EXPECT_EQ(v3[0], v1[0] - v2[0]) << "Erreur de decrement pour un vecteur de dimension 1";

    Vecteur<2> v4 = {-1.0, 1.2};
    Vecteur<2> v5 = {90.0, 78.65};
    Vecteur<2> v6 = v4;
    v6 -= v5;
    EXPECT_EQ(v6[0], v4[0] - v5[0]) << "Erreur de decrement pour un vecteur de dimension 2 pour la premiere composante";
    EXPECT_EQ(v6[1], v4[1] - v5[1]) << "Erreur de decrement pour un vecteur de dimension 2 pour la seconde composante";

    Vecteur<3> v7 = {-12.0, 1.23, 1.45};
    Vecteur<3> v8 = {84.0, 8744.0, 0.4855};
    Vecteur<3> v9 = v7;
    v9 -= v8;
    EXPECT_EQ(v9[0], v7[0] - v8[0]) << "Erreur de decrement pour un vecteur de dimension 3 pour la premiere composante";
    EXPECT_EQ(v9[1], v7[1] - v8[1]) << "Erreur de decrement pour un vecteur de dimension 3 pour la seconde composante";
    EXPECT_EQ(v9[2], v7[2] - v8[2]) << "Erreur de decrement pour un vecteur de dimension 3 pour la troisième composante";
}

TEST(VecteurTests, addition){
    Vecteur<1> v1 = {-2.0};
    Vecteur<1> v2 = {56.6};
    Vecteur<1> v3 = v1 + v2;
    EXPECT_EQ(v3[0], v1[0] + v2[0]) << "Erreur d'addition pour un vecteur de dimension 1";

    Vecteur<2> v4 = {-1.0, 1.2};
    Vecteur<2> v5 = {90.0, 78.65};
    Vecteur<2> v6 = v4 + v5;
    EXPECT_EQ(v6[0], v4[0] + v5[0]) << "Erreur d'addition pour un vecteur de dimension 2 pour la premiere composante";
    EXPECT_EQ(v6[1], v4[1] + v5[1]) << "Erreur d'addition pour un vecteur de dimension 2 pour la seconde composante";

    Vecteur<3> v7 = {-12.0, 1.23, 1.45};
    Vecteur<3> v8 = {84.0, 8744.0, 0.4855};
    Vecteur<3> v9 = v7 + v8;
    EXPECT_EQ(v9[0], v7[0] + v8[0]) << "Erreur d'addition pour un vecteur de dimension 3 pour la premiere composante";
    EXPECT_EQ(v9[1], v7[1] + v8[1]) << "Erreur d'addition pour un vecteur de dimension 3 pour la seconde composante";
    EXPECT_EQ(v9[2], v7[2] + v8[2]) << "Erreur d'addition pour un vecteur de dimension 3 pour la troisième composante";
}

TEST(VecteurTests, produit){
    Vecteur<1> v1 = {-2.0};
    Vecteur<1> v2 = 5.265*v1;
    EXPECT_EQ(v2[0], 5.265*v1[0]) << "Erreur de produit pour un vecteur de dimension 1";

    Vecteur<2> v3 = {-1.0, 1.2};
    Vecteur<2> v4 = 15.15*v3;
    EXPECT_EQ(v4[0], 15.15*v3[0]) << "Erreur de produit pour un vecteur de dimension 2 pour la premiere composante";
    EXPECT_EQ(v4[1], 15.15*v3[1]) << "Erreur de produit pour un vecteur de dimension 2 pour la seconde composante";

    Vecteur<3> v5 = {-12.0, 1.23, 1.45};
    Vecteur<3> v6 = 5.5*v5;
    EXPECT_EQ(v6[0], 5.5*v5[0]) << "Erreur de produit pour un vecteur de dimension 3 pour la premiere composante";
    EXPECT_EQ(v6[1], 5.5*v5[1]) << "Erreur de produit pour un vecteur de dimension 3 pour la seconde composante";
    EXPECT_EQ(v6[2], 5.5*v5[2]) << "Erreur de produit pour un vecteur de dimension 3 pour la troisième composante";
}

TEST(VecteurTests, soustraction){
    Vecteur<1> v1 = {-2.0};
    Vecteur<1> v2 = {56.6};
    Vecteur<1> v3 = v1 - v2;
    EXPECT_EQ(v3[0], v1[0] - v2[0]) << "Erreur de soustraction pour un vecteur de dimension 1";

    Vecteur<2> v4 = {-1.0, 1.2};
    Vecteur<2> v5 = {90.0, 78.65};
    Vecteur<2> v6 = v4 - v5;
    EXPECT_EQ(v6[0], v4[0] - v5[0]) << "Erreur de soustraction pour un vecteur de dimension 2 pour la premiere composante";
    EXPECT_EQ(v6[1], v4[1] - v5[1]) << "Erreur de soustraction pour un vecteur de dimension 2 pour la seconde composante";

    Vecteur<3> v7 = {-12.0, 1.23, 1.45};
    Vecteur<3> v8 = {84.0, 8744.0, 0.4855};
    Vecteur<3> v9 = v7 - v8;
    EXPECT_EQ(v9[0], v7[0] - v8[0]) << "Erreur de soustraction pour un vecteur de dimension 3 pour la premiere composante";
    EXPECT_EQ(v9[1], v7[1] - v8[1]) << "Erreur de soustraction pour un vecteur de dimension 3 pour la seconde composante";
    EXPECT_EQ(v9[2], v7[2] - v8[2]) << "Erreur de soustraction pour un vecteur de dimension 3 pour la troisième composante";
}

TEST(VecteurTests, accesLecture){
    Vecteur<1> v1 = {-2.0};
    EXPECT_EQ(v1[0], v1(1)) << "Erreur d'acces en lecture pour un vecteur de dimension 1";

    Vecteur<2> v2 = {-1.0, 1.2};
    EXPECT_EQ(v2[0], v2(1)) << "Erreur d'acces en lecture pour un vecteur de dimension 2 pour la premiere composante";
    EXPECT_EQ(v2[1], v2(2)) << "Erreur d'acces en lecture pour un vecteur de dimension 2 pour la seconde composante";

    Vecteur<3> v3 = {-12.0, 1.23, 1.45};
    EXPECT_EQ(v3[0], v3(1)) << "Erreur d'acces en lecture pour un vecteur de dimension 3 pour la premiere composante";
    EXPECT_EQ(v3[1], v3(2)) << "Erreur d'acces en lecture pour un vecteur de dimension 3 pour la seconde composante";
    EXPECT_EQ(v3[2], v3(3)) << "Erreur d'acces en lecture pour un vecteur de dimension 3 pour la troisième composante";
}

TEST(VecteurTests, accesEcriture){
    Vecteur<1> v1 = {0.0};
    v1[0] = -2.0;
    EXPECT_EQ(v1[0], -2.0) << "Erreur d'acces informatique en ecriture pour un vecteur de dimension 1";
    v1(1) = -10.0;
    EXPECT_EQ(v1[0], -10.0) << "Erreur d'acces mathematique en ecriture pour un vecteur de dimension 1";

    Vecteur<2> v2 = {-1.0, 1.2};
    v2[0] = 5.0;
    v2[1] = 485.5;
    EXPECT_EQ(v2[0], 5.0) << "Erreur d'acces informatique en ecriture pour un vecteur de dimension 2 pour la premiere composante";
    EXPECT_EQ(v2[1], 485.5) << "Erreur d'acces informatique en ecriture pour un vecteur de dimension 2 pour la seconde composante";
    v2(1) = 456.0;
    v2(2) = 897.2;
    EXPECT_EQ(v2[0], 456.0) << "Erreur d'acces mathematique en ecriture pour un vecteur de dimension 2 pour la premiere composante";
    EXPECT_EQ(v2[1], 897.2) << "Erreur d'acces mathematique en ecriture pour un vecteur de dimension 2 pour la seconde composante";

    Vecteur<3> v3 = {-12.0, 1.23, 1.45};
    v3[0] = 48.0;
    v3[1] = 98.3;
    v3[2] = 74.3;
    EXPECT_EQ(v3[0], 48.0) << "Erreur d'acces informatique en ecriture pour un vecteur de dimension 3 pour la premiere composante";
    EXPECT_EQ(v3[1], 98.3) << "Erreur d'acces informatique en ecriture pour un vecteur de dimension 3 pour la seconde composante";
    EXPECT_EQ(v3[2], 74.3) << "Erreur d'acces informatique en ecriture pour un vecteur de dimension 3 pour la troisième composante";
    v3(1) = 6.1;
    v3(2) = 7.0;
    v3(3) = 89.1;
    EXPECT_EQ(v3[0], 6.1) << "Erreur d'acces mathematique en ecriture pour un vecteur de dimension 3 pour la premiere composante";
    EXPECT_EQ(v3[1], 7.0) << "Erreur d'acces mathematique en ecriture pour un vecteur de dimension 3 pour la seconde composante";
    EXPECT_EQ(v3[2], 89.1) << "Erreur d'acces mathematique en ecriture pour un vecteur de dimension 3 pour la troisième composante";
}

TEST(VecteurTests, produitScalaire){
    Vecteur<1> v1 = {-2.0};
    Vecteur<1> v2 = {-10.0};
    EXPECT_EQ(produit_scalaire_euclidien(v1, v2), -2.0*-10.0) << "Erreur de produit scalaire pour un vecteur de dimension 1";
    
    Vecteur<2> v3 = {-1.0, 1.2};
    Vecteur<2> v4 = {456.0, 897.2};
    EXPECT_EQ(produit_scalaire_euclidien(v3, v4), -1.0*456.0+1.2*897.2) << "Erreur de produit scalaire pour un vecteur de dimension 2";

    Vecteur<3> v5 = {-12.0, 1.23, 1.45};
    Vecteur<3> v6 = {6.1, 7.0, 89.1};
    EXPECT_EQ(produit_scalaire_euclidien(v5, v6), -12.0*6.1+1.23*7.0+1.45*89.1) << "Erreur de produit scalaire pour un vecteur de dimension 3";
}

TEST(VecteurTests, norme){
    Vecteur<1> v1 = {-2.0};
    EXPECT_EQ(v1.norme_2(), sqrt(pow(-2.0,2))) << "Erreur de norme pour le vecteur de dimension 1 : " << v1 ;
    Vecteur<1> v2 = {-10.0};
    EXPECT_EQ(v2.norme_2(), sqrt(pow(-10.0,2))) << "Erreur de norme pour le vecteur de dimension 1 : " << v2;
    
    Vecteur<2> v3 = {-1.0, 1.2};
    EXPECT_EQ(v3.norme_2(), sqrt(pow(-1.0,2)+pow(1.2,2))) << "Erreur de norme pour le vecteur de dimension 2 : " << v3;
    Vecteur<2> v4 = {456.0, 897.2};
    EXPECT_EQ(v4.norme_2(), sqrt(pow(456.0,2)+pow(897.2,2))) << "Erreur de norme pour le vecteur de dimension 2" << v4;

    Vecteur<3> v5 = {-12.0, 1.23, 1.45};
    EXPECT_EQ(v5.norme_2(), sqrt(pow(-12.0,2)+pow(1.23,2)+pow(1.45,2))) << "Erreur de norme pour le vecteur de dimension 3 : " << v5;
    Vecteur<3> v6 = {6.1, 7.0, 89.1};
    EXPECT_EQ(v6.norme_2(), sqrt(pow(6.1,2)+pow(7.0,2)+pow(89.1,2))) << "Erreur de norme pour le vecteur de dimension 3 : " << v6;
}

TEST(VecteurTests, distance){
    Vecteur<1> v1 = {-2.0};
    Vecteur<1> v2 = {-10.0};
    EXPECT_EQ(distance_euclidienne(v1,v2), sqrt(pow(-2.0-(-10.0),2))) << "Erreur de distance pour un vecteur de dimension 1 : ";
    
    Vecteur<2> v3 = {-1.0, 1.2};
    Vecteur<2> v4 = {456.0, 897.2};
    EXPECT_EQ(distance_euclidienne(v3,v4), sqrt(pow(-1.0-456.0,2)+pow(1.2-897.2,2))) << "Erreur de distance pour un vecteur de dimension 2";

    Vecteur<3> v5 = {-12.0, 1.23, 1.45};
    Vecteur<3> v6 = {6.1, 7.0, 89.1};
    EXPECT_EQ(distance_euclidienne(v5,v6), sqrt(pow(-12.0-6.1,2)+pow(1.23-7.0,2)+pow(1.45-89.1,2))) << "Erreur de distance pour un vecteur de dimension 3 : ";
}

// Test des particules
#include "particules.hpp"
TEST(ParticuleTests, creation){
    Particule<3> part = Particule(2.0, Vecteur<3>({0.0,0.0,0.0}), Vecteur<3>({10.0,10.0,10.0}), Vecteur<3>({100.0,100.0,100.0}), 3, 4);
    EXPECT_EQ(part.getMasse(), 2.0) << "Erreur initialisation de la masse";
    EXPECT_EQ(part.getPosition(), Vecteur<3>({0.0,0.0,0.0})) << "Erreur initialisation de la position";
    EXPECT_EQ(part.getVitesse(), Vecteur<3>({10.0,10.0,10.0})) << "Erreur initialisation de la vitesse";
    EXPECT_EQ(part.getForce(), Vecteur<3>({100.0,100.0,100.0})) << "Erreur initialisation de la force";
    EXPECT_EQ(part.getIdentifiant(), 3) << "Erreur initialisation de l'identifiant";
    EXPECT_EQ(part.getCategorie(), 4) << "Erreur initialisation de la categorie";
}

TEST(ParticuleTests, AccesseursMutateurs){
    Particule<3> part = Particule(0.0, Vecteur<3>({0.0,0.0,0.0}), Vecteur<3>({0.0,0.0,0.0}), Vecteur<3>({0.0,0.0,0.0}), 0, 0);
    // Masse
    EXPECT_EQ(part.getMasse(), 0.0) << "Erreur d'acces à la masse";
    part.setMasse(5.0);
    EXPECT_EQ(part.getMasse(), 5.0) << "Erreur de modification de la masse";

    // Position
    EXPECT_EQ(part.getPosition(), Vecteur<3>({0.0,0.0,0.0})) << "Erreur d'acces à la position";
    part.setPosition(Vecteur<3>({6.0,7.0,8.0}));
    EXPECT_EQ(part.getPosition(), Vecteur<3>({6.0,7.0,8.0})) << "Erreur de modification de la position";

    // Vitesse
    EXPECT_EQ(part.getVitesse(), Vecteur<3>({0.0,0.0,0.0})) << "Erreur d'acces à la vitesse";
    part.setVitesse(Vecteur<3>({9.0,10.0,11.0}));
    EXPECT_EQ(part.getVitesse(), Vecteur<3>({9.0,10.0,11.0})) << "Erreur de modification de la vitesse";

    // Force
    EXPECT_EQ(part.getForce(), Vecteur<3>({0.0,0.0,0.0})) << "Erreur d'acces à la force";
    part.setForce(Vecteur<3>({12.0,13.0,14.0}));
    EXPECT_EQ(part.getForce(), Vecteur<3>({12.0,13.0,14.0})) << "Erreur de modification de la force";

    // Identifiant
    EXPECT_EQ(part.getIdentifiant(), 0) << "Erreur d'acces à l'identifiant";
    part.setIdentifiant(15);
    EXPECT_EQ(part.getIdentifiant(), 15) << "Erreur de modification de l'identifiant";

    // Categorie
    EXPECT_EQ(part.getCategorie(), 0) << "Erreur d'acces à la categorie";
    part.setCategorie(16);
    EXPECT_EQ(part.getCategorie(), 16) << "Erreur de modification de la categorie";
}

TEST(ParticuleTests, gravitee){
    Particule<3> part1 = Particule(1.0, Vecteur<3>({0.0, 0.0, 0.0}), Vecteur<3>({0.0, 0.0, 0.0}), Vecteur<3>({0.0, 0.0, 0.0}), 0, 0);
    Particule<3> part2 = Particule(1.0, Vecteur<3>({1.0, 0.0, 0.0}), Vecteur<3>({0.0, 0.0, 0.0}), Vecteur<3>({0.0, 0.0, 0.0}), 0, 0);
    EXPECT_EQ(potentiel_gravitationelle(part1, part2), -1) << "Erreur de potentiel gravitationnel pour les particules" << part1 << " et " << part2;
    EXPECT_EQ(force_gravitationelle(part1, part2), Vecteur<3>({1.0, 0.0, 0.0})) << "Erreur de force gravitationnelle pour les particules" << part1 << " et " << part2;
    
    part1.setMasse(2.0);
    EXPECT_EQ(potentiel_gravitationelle(part1, part2), -2) << "Erreur de potentiel gravitationnel pour les particules" << part1 << " et " << part2;
    EXPECT_EQ(force_gravitationelle(part1, part2), Vecteur<3>({2.0, 0.0, 0.0})) << "Erreur de force gravitationnelle pour les particules" << part1 << " et " << part2;

    part1.setPosition(Vecteur<3>({-1.0, 0.0, 0.0}));
    EXPECT_EQ(potentiel_gravitationelle(part1, part2), -1.0) << "Erreur de potentiel gravitationnel pour les particules" << part1 << " et " << part2;
    EXPECT_EQ(force_gravitationelle(part1, part2), Vecteur<3>({0.5, 0.0, 0.0})) << "Erreur de force gravitationnelle pour les particules" << part1 << " et " << part2;

    part1.setMasse(1.0);
    part1.setPosition(Vecteur<3>({0.0, 0.0, 0.0}));
    part2.setPosition(Vecteur<3>({0.0, 1.0, 0.0}));
    EXPECT_EQ(potentiel_gravitationelle(part1, part2), -1.0) << "Erreur de potentiel gravitationnel pour les particules" << part1 << " et " << part2;
    EXPECT_EQ(force_gravitationelle(part1, part2), Vecteur<3>({0.0, 1.0, 0.0})) << "Erreur de force gravitationnelle pour les particules" << part1 << " et " << part2;

    part2.setPosition(Vecteur<3>({0.0,0.0,1.0}));
    EXPECT_EQ(potentiel_gravitationelle(part1, part2), -1.0) << "Erreur de potentiel gravitationnel pour les particules" << part1 << " et " << part2;
    EXPECT_EQ(force_gravitationelle(part1, part2), Vecteur<3>({0.0, 0.0, 1.0})) << "Erreur de force gravitationnelle pour les particules" << part1 << " et " << part2;

    part2.setPosition(Vecteur<3>({1.0,0.0,1.0}));
    EXPECT_EQ(potentiel_gravitationelle(part1, part2), -1.0/sqrt(2.0)) << "Erreur de potentiel gravitationnel pour les particules" << part1 << " et " << part2;
    EXPECT_LE((force_gravitationelle(part1, part2)-Vecteur<3>({1.0/sqrt(2.0)/2.0, 0.0, 1.0/sqrt(2.0)/2.0})).norme_2(), 1e-10) << "Erreur de force gravitationnelle pour les particules" << part1 << " et " << part2;

}

TEST(ParticuleTests, LennardJones){
    Particule<3> part1 = Particule(1.0, Vecteur<3>({0.0, 0.0, 0.0}), Vecteur<3>({0.0, 0.0, 0.0}), Vecteur<3>({0.0, 0.0, 0.0}), 0, 0);
    Particule<3> part2 = Particule(1.0, Vecteur<3>({1.0, 0.0, 0.0}), Vecteur<3>({0.0, 0.0, 0.0}), Vecteur<3>({0.0, 0.0, 0.0}), 0, 0);
    EXPECT_EQ(potentiel_lennard_jones(1.0, 1.0, part1, part2), 0) << "Erreur de potentiel de Lennard-Jones pour les particules" << part1 << " et " << part2;
    
    part1.setMasse(2.0);
    EXPECT_EQ(potentiel_lennard_jones(1.0, 1.0, part1, part2), 0) << "Erreur de potentiel de Lennard-Jones pour les particules" << part1 << " et " << part2;

    part1.setMasse(1.0);
    part2.setPosition(Vecteur<3>({0.0, 1.0, 0.0}));
    EXPECT_EQ(potentiel_lennard_jones(1.0, 1.0, part1, part2), 0) << "Erreur de potentiel de Lennard-Jones pour les particules" << part1 << " et " << part2;
    
    part2.setPosition(Vecteur<3>({0.0,0.0,1.0}));
    EXPECT_EQ(potentiel_lennard_jones(1.0, 1.0, part1, part2), 0) << "Erreur de potentiel de Lennard-Jones pour les particules" << part1 << " et " << part2;
    
    part2.setPosition(Vecteur<3>({1/sqrt(2.0),0.0,1/sqrt(2.0)}));
    EXPECT_LE(abs(potentiel_lennard_jones(1.0, 1.0, part1, part2)), 1e-10) << "Erreur de potentiel de Lennard-Jones pour les particules" << part1 << " et " << part2;
    
    part2.setPosition(Vecteur<3>({pow(2, 1.0/6.0), 0.0, 0.0}));
    EXPECT_LE(force_lennard_jones(1.0, 1.0, part1, part2).norme_2(), 1e-10) << "Erreur de force de Lennard-Jones pour les particules" << part1 << " et " << part2;
    
    part2.setPosition(Vecteur<3>({pow(2, -1.0/3.0), pow(2, -1.0/3.0), 0.0}));
    EXPECT_LE(force_lennard_jones(1.0, 1.0, part1, part2).norme_2(), 1e-10) << "Erreur de force de Lennard-Jones pour les particules" << part1 << " et " << part2;

    part1.setPosition(Vecteur<3>({0.0,0.0,-1.0}));
    part2.setPosition(Vecteur<3>({0.0,0.0,1.0}));
    EXPECT_LE(force_lennard_jones(1.0, pow(2.0, 5.0/6.0), part1, part2).norme_2(), 1e-10) << "Erreur de force de Lennard-Jones pour les particules" << part1 << " et " << part2 << " avec un sigma approprie pour 0";
    EXPECT_LE((force_lennard_jones(1.0/(24.0*pow(1.0/2.0, 6)*(1-pow(1.0/2.0,5)) / 2.0), 1.0, part1, part2) - Vecteur<3>({0.0,0.0,1.0})).norme_2(), 1e-10) << "Erreur de force de Lennard-Jones pour les particules" << part1 << " et " << part2 << " avec un epsilon approprie pour 1";
}

TEST(ParticuleTests, EnergieCinetique){
    Particule<3> part = Particule(1.0, Vecteur<3>({0.0, 0.0, 0.0}), Vecteur<3>({0.0, 0.0, 0.0}), Vecteur<3>({0.0, 0.0, 0.0}), 0, 0);

    EXPECT_EQ(part.energie_cinetique(), 0) << "Erreur energie cinetique avec vitesse nulle";

    part.setVitesse(Vecteur<3>({0.0,0.0,-1.0}));
    EXPECT_EQ(part.energie_cinetique(), 1.0/2.0) << "Erreur energie cinetique avec vitesse sur une composante";

    part.setVitesse(Vecteur<3>({0.0,2.0,-1.0}));
    EXPECT_LE(abs(part.energie_cinetique() - 5.0 / 2.0), 1e-10) << "Erreur energie cinétique avec vitesse composee";

    part.setVitesse(Vecteur<3>({1.0,0.0,0.0}));
    part.setMasse(15.0);
    EXPECT_LE(abs(part.energie_cinetique() - 15.0 / 2.0), 1e-10) << "Erreur energie cinétique avec masse differente";

    part.setMasse(0.0);
    EXPECT_EQ(part.energie_cinetique(), 0) << "Erreur energie cinetique avec masse nulle";
}

// Test de l'univers
#include "univers.hpp"