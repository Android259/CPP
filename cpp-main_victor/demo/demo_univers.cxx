#include <iostream>
#include <fstream>
#include <chrono>  // Pour la mesure du temps
#include "univers.hpp"

using namespace std;

void test_systeme_solaire(){
    Vecteur<2> decalage = {500.0,500.0};

    double m_soleil = 1e14;
    Vecteur<2> p_soleil = {0, 0}, v_soleil = {0, 0}, f_soleil = {0, 0};
    p_soleil += decalage;
    Particule part_soleil(m_soleil, p_soleil, v_soleil, f_soleil, 0, 0);

    double m_terre = 3.0e8;
    Vecteur<2> p_terre = {0, 1}, v_terre = {0, -0.01}, f_terre = {0, 0};
    p_terre += decalage;
    Particule part_terre(m_terre, p_terre, v_terre, f_terre, 1, 0);

    double m_jupiter = 9.55e10;
    Vecteur<2> p_jupiter = {5.36, 0}, v_jupiter = {0, 0}, f_jupiter = {0, 0};
    p_jupiter += decalage;
    Particule part_jupiter(m_jupiter, p_jupiter, v_jupiter, f_jupiter, 2, 0);

    double m_haley = 1;
    Vecteur<2> p_haley = {34.75, 0}, v_haley = {0, 0}, f_haley = {0, 0};
    p_haley += decalage;
    Particule part_haley(m_haley, p_haley, v_haley, f_haley, 3, 0);

    std::list<Particule<2>> listeParticule;
    listeParticule.push_front(part_haley);
    listeParticule.push_front(part_jupiter);
    listeParticule.push_front(part_terre);
    listeParticule.push_front(part_soleil);

    Vecteur<2> longueurs_caracteristiques = {1000, 1000};
    Univers<2> u = Univers(1000.0, 1.0, 1.0, longueurs_caracteristiques, listeParticule);

    // Ouvrir le fichier en écriture (le crée s'il n'existe pas)
    std::ofstream fichier("planettes.txt");

    // Vérifier si le fichier est bien ouvert
    if (!fichier.is_open()) {
        return;
    }

    double t = 0;
    double dt = 0.015, tend = 1000;
    int i = 0;
    while (t < tend){
        if (t >= i * tend / 100.0) {
            u.afficher_position(fichier);
            fichier << "em : " << u.energie_mecanique() << endl;
            std::cout << i << "%" << std::endl;
            i++;
        }
        
        u.stormer_verlet_iteration(dt, t);
    }
    u.afficher_position(fichier);
    std::cout << u << std::endl;
    fichier.close();
}

void test_collision(){
    auto start = std::chrono::steady_clock::now();

    Vecteur<2> longueurs_caracteristiques = {250, 250};
    double epsilon = 5.0, sigma = 1.0;
    double r_coupure = 2.5 * sigma;
    double masse = 1.0;
    double dt = 0.00005, t_fin = 19.5;

    Vecteur<2> decalage = {125, 125};
    double dist_points = pow(2, 1.0/6.0);
    std::list<Particule<2>> listeParticule;
    Vecteur<2> force = {0.0, 0.0};
    // Carré rouge
    
    Vecteur<2> vitesse_rouge = {0.0, -10.0};
    Vecteur<2> decalage_rouge = {0.0, 30.0};
    decalage_rouge += decalage;
    for (int i = 0; i < 20; i++){
        for (int j = 0; j < 20; j++){
            Vecteur<2> point;
            point[0] = dist_points / 2.0 + i * dist_points;
            point[1] = dist_points / 2.0 + j * dist_points;
            point += decalage_rouge;
            Particule part(masse, point, vitesse_rouge, force, 0, 0);
            listeParticule.push_front(part);
        }
    }
    for (int i = 0; i < 20; i++){
        for (int j = 0; j < 20; j++){
            Vecteur<2> point;
            point[0] = -(dist_points / 2.0 + i * dist_points);
            point[1] = dist_points / 2.0 + j * dist_points;
            point += decalage_rouge;
            Particule part(masse, point, vitesse_rouge, force, 0, 0);
            listeParticule.push_front(part);
        }
    }
    for (int i = 0; i < 20; i++){
        for (int j = 0; j < 20; j++){
            Vecteur<2> point;
            point[0] = dist_points / 2.0 + i * dist_points;
            point[1] = -(dist_points / 2.0 + j * dist_points);
            point += decalage_rouge;
            Particule part(masse, point, vitesse_rouge, force, 0, 0);
            listeParticule.push_front(part);
        }
    }
    for (int i = 0; i < 20; i++){
        for (int j = 0; j < 20; j++){
            Vecteur<2> point;
            point[0] = -(dist_points / 2.0 + i * dist_points);
            point[1] = -(dist_points / 2.0 + j * dist_points);
            point += decalage_rouge;
            Particule part(masse, point, vitesse_rouge, force, 0, 0);
            listeParticule.push_front(part);
        }
    }

    // Rectangle bleu
    Vecteur<2> vitesse_bleu = {0.0, 0.0};
    Vecteur<2> decalage_bleu = {0.0, -30.0};
    decalage_bleu += decalage;
    for (int i = 0; i < 80; i++){
        for (int j = 0; j < 20; j++){
            Vecteur<2> point;
            point[0] = dist_points / 2.0 + i * dist_points;
            point[1] = dist_points / 2.0 + j * dist_points;
            point += decalage_bleu;
            Particule part(masse, point, vitesse_bleu, force, 0, 0);
            listeParticule.push_front(part);
        }
    }
    for (int i = 0; i < 80; i++){
        for (int j = 0; j < 20; j++){
            Vecteur<2> point;
            point[0] = -(dist_points / 2.0 + i * dist_points);
            point[1] = dist_points / 2.0 + j * dist_points;
            point += decalage_bleu;
            Particule part(masse, point, vitesse_bleu, force, 0, 0);
            listeParticule.push_front(part);
        }
    }
    for (int i = 0; i < 80; i++){
        for (int j = 0; j < 20; j++){
            Vecteur<2> point;
            point[0] = dist_points / 2.0 + i * dist_points;
            point[1] = -(dist_points / 2.0 + j * dist_points);
            point += decalage_bleu;
            Particule part(masse, point, vitesse_bleu, force, 0, 0);
            listeParticule.push_front(part);
        }
    }
    for (int i = 0; i < 80; i++){
        for (int j = 0; j < 20; j++){
            Vecteur<2> point;
            point[0] = -(dist_points / 2.0 + i * dist_points);
            point[1] = -(dist_points / 2.0 + j * dist_points);
            point += decalage_bleu;
            Particule part(masse, point, vitesse_bleu, force, 0, 0);
            listeParticule.push_front(part);
        }
    }


    Univers<2> u = Univers(r_coupure, epsilon, sigma, longueurs_caracteristiques, listeParticule);
    // Ouvrir le fichier en écriture (le crée s'il n'existe pas)
    std::ofstream fichier("collision.txt");

    // Vérifier si le fichier est bien ouvert
    if (!fichier.is_open()) {
        return;
    }

    double t = 0;
    int i = 0;
    double grain_affichage = 1 / 10000.0;
    while (t < t_fin){
        //std::cout << t << std::endl;
        if (t >= i * t_fin * grain_affichage) {
            u.afficher_position(fichier);
            fichier << "em : " << u.energie_mecanique() << endl;
            std::cout << t / t_fin * 100 << "%" << ", nb : " << u.nombre_particules() << std::endl;
            i++;
        }
        u.stormer_verlet_iteration(dt, t);
    }
    u.afficher_position(fichier);
    fichier.close();

    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::cout << "temps d'execution : " << elapsed_seconds.count() << "s\n";
}

int main() {
    //test_systeme_solaire();
    test_collision();
}