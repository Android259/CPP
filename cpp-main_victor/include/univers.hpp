#ifndef UNIVERS_HPP
#define UNIVERS_HPP

#include <array>   // pour les std::array
#include <functional>
#include <iostream>

#include "vecteur.hpp"
#include "particules.hpp"

template <int Dim>
using Cellule = std::list<Particule<Dim>>;

/// @brief Une classe qui represente un univers fini de dimension Dim
template <int Dim>
class Univers {
   private:
    /// Le rayon de coupure dans l'univers
    double rayon_coupure;

    /// Les longueurs de l'univers dans chaque dimension
    Vecteur<Dim> longueurs_caracteristiques;

    /// Le nombre de cellules sur chaque dimension
    std::array<int, Dim> nombre_cellules_dimension;

    /// Le nombre total de cellules
    int nombre_cellules;

    /// Un tableau dynamique de cellules
    Cellule<Dim>* cellules;

    /// Termes epsilon et sigma pour les interactions de Lennard-Jones
    double epsilon;
    double sigma;

   public:
    /// @brief Calcule l'indice de la cellule qui contient la position dans un tableau de cellules de dimension Dim
    /// @param pos la position de la particule dont on cherche a determiner l'indice de la cellule
    /// @return l'indice de la cellule dans laquelle est la particule à la position pos
    int indice_n_dimensions(const Vecteur<Dim>& pos){
        if (Dim == 0){
            return 0;
        }
        int i = Dim - 1;
        int indice = (static_cast<int>(pos[Dim-1] / this->rayon_coupure));
        while (i > 0){
            indice *= this->nombre_cellules_dimension[i];
            i--;
            indice += (static_cast<int>(pos[i] / this->rayon_coupure));
        }
        return indice;
    }

    /// @brief constructeur de l'univers avec la plupart des attributs et une liste de particules
    Univers(double r_coupure, double epsilon, double sigma, const Vecteur<Dim>& L_caracteristiques, const std::list<Particule<Dim>>& listePart){
        this->rayon_coupure = r_coupure;
        this->epsilon = epsilon;
        this->sigma = sigma;
        this->longueurs_caracteristiques = L_caracteristiques;
        this->nombre_cellules = 1;
        for (int i = 0; i < Dim; i++){
            this->nombre_cellules_dimension[i] = static_cast<int>(this->longueurs_caracteristiques[i] / rayon_coupure);
            this->nombre_cellules *= this->nombre_cellules_dimension[i];
        }
        this->cellules = new Cellule<Dim>[this->nombre_cellules];
        for (auto part : listePart){
            int indice = this->indice_n_dimensions(part.getPosition());
            this->cellules[indice].push_front(part);
        }
    }

    /// @brief le destructeur de l'univers
    /// @details il faut juste détruire le tableau dynamique des cellules
    ~Univers(){
        delete[] this->cellules;
    }

    /// @brief Appel la fonction sur chacune des particules
    /// @param fonction la fonction qui doit être appelée sur chacune des cellules
    void parcoure_particules(std::function<void(Particule<Dim>*)> fonction){
        for (int i = 0; i < this->nombre_cellules; i++){
            for (auto& part : this->cellules[i]){
                fonction(&part);
            }
        }
    }

    /// @brief Appel la fonction sur chaque couple de particules qui sont dans les cellules voisines l'une de l'autre
    /// @details la fonction est appelée une fois sur chaque couple
    void parcoure_couple_particules_proches(std::function<void(Particule<Dim>*, Particule<Dim>*)> fonction){
        
        int d = 0;
        // On determine les poids de chaque dimension dans la tableau linéarisé de cellules
        std::array<int, Dim> poids = {};
        poids[d] = 1;
        while(d < Dim - 1){
            poids[d+1] = poids[d];
            poids[d+1] *= this->nombre_cellules_dimension[d];
            d++;
        }
        // indice_dimension_cellule est l'indice de la cellule en dimension Dim, il commence avec toutes les composantes à zero
        std::array<int, Dim> indice_dimension_cellule = {};

        for (int i = 0; i < this->nombre_cellules; i++){
            // On appel la fonction pour les couples de particules dans cellule d'indice i
            for (auto it_particule_1 = begin(this->cellules[i]);
                it_particule_1 != end(this->cellules[i]); it_particule_1++) {
                for (auto it_particule_2 = next(it_particule_1);
                    it_particule_2 != end(this->cellules[i]); it_particule_2++) {
                        fonction(&(*it_particule_1), &(*it_particule_2));
                }
            }
            // Pour les couples avec les cellules suivantes (pour que chaque couple ne soit appelé qu'une seule fois, on a indice_decalage > 0)
            int indice_decalage = 0; // L'indice de décalage (différence d'indice avec la cellule)
            // On maintient un tableau pour suivre les décalages d'indice en dimension Dim, il commence avec toutes ses composantes à zero
            std::array<int, Dim> indice_pris = {};
            for (d = 0; d < Dim; d++){
                indice_pris[d] = 0;
            }
            // On determine la première dimension sur laquelle il y a un voisin suivant
            int d0 = 0;
            while (d0 < Dim && indice_dimension_cellule[d0] == this->nombre_cellules_dimension[d0] - 1){ // 
                d0++;
            }

            d = d0;
            while(d < Dim){ // tant que l'on ne cherche pas à incrémenter pour une dimension supérieur à la dimension de l'univers
                // On incrémente la dimension d (sur l'indice en dimension d et le décalage d'indice)
                indice_pris[d] += 1;
                indice_decalage += poids[d];
                // On itere la fonction sur les deux bonnes cellules
                for (auto& particule_1 : this->cellules[i]){
                    for (auto& particule_2 : this->cellules[i+indice_decalage]){
                        fonction(&particule_1, &particule_2);
                    }
                }
                // On determine le prochain indice de décalage
                d = d0;
                while (d < Dim && indice_pris[d] == 1){
                    indice_pris[d] = -1;
                    indice_decalage -= 2 * poids[d];
                    d++;
                    while (indice_dimension_cellule[d] == this->nombre_cellules_dimension[d] - 1){
                        d++;
                    }
                }
            }
            // On met a jour l'indice de la cellule
            d = 0;
            indice_dimension_cellule[d]++;
            while (d < Dim - 1 && indice_dimension_cellule[d] == this->nombre_cellules_dimension[d]){
                indice_dimension_cellule[d] -= this->nombre_cellules_dimension[d];
                indice_dimension_cellule[d+1]++;
                d++;
            }
        }
    }

    /// @brief Calcule les forces qui s'appliquent sur chacune des particules
    /// @todo voir si l'on fait la coupure avec le rayon de coupure et si oui, si on la fait dans la calcule de la force ou dans le parcours
    /// @details La force est ici la force de Lennard-Jones
    void calcule_force_interaction() {
        // Met a zero toutes les forces
        Vecteur<Dim> z = Vecteur<Dim>(0.0);
        Vecteur<Dim>& zp = z;
        this->parcoure_particules([zp](Particule<Dim>* part){part->setForce(zp);});
        // Calcule les forces
        double epsilon = this->epsilon, sigma = this->sigma;
        this->parcoure_couple_particules_proches([epsilon, sigma](Particule<Dim>* particule_1, Particule<Dim>* particule_2){
            Vecteur<Dim> f = force_lennard_jones(epsilon, sigma, *particule_1, *particule_2);
            particule_1->setForce(particule_1->getForce() + f);
            particule_2->setForce(particule_2->getForce() - f);
        });
    }
    
    /// @brief Calcule les forces qui s'appliquent sur chacune des particules
    /// @todo voir si l'on fait la coupure avec le rayon de coupure et si oui, si on la fait dans la calcule du potentiel ou dans le parcours
    /// @details Le potentiel est ici le potentiel de Lennard-Jones
    double energie_mecanique(){
        double em = 0;
        // Ajoute l'énergie potentielle de chaque liaison
        double epsilon = this->epsilon, sigma = this->sigma;
        this->parcoure_couple_particules_proches([&em, epsilon, sigma](Particule<Dim>* particule_1, Particule<Dim>* particule_2){
            em += 2 * potentiel_lennard_jones(epsilon, sigma, *particule_1, *particule_2);
        });
        // Ajoute l'énergie cinétique de chaque particule
        this->parcoure_particules([&em](Particule<Dim>* part){em += part->energie_cinetique();});

        return em;
    }
    
    /// @brief Met a jour la répartition dans les cellules en fonction des positions des particules
    /// @details Si la particule n'est plus dans l'univers la supprime
    /// @todo Voir si l'affichage d'une erreur est pertinent
    void mettre_a_jour_cellules(){
        for (int i = 0; i < this->nombre_cellules; i++){
            auto it_particule = begin(this->cellules[i]);
            while (it_particule != end(this->cellules[i])){
                bool dans_univers = true;
                int d = 0;
                while (d < Dim && dans_univers){
                    dans_univers = dans_univers && (*it_particule).getPosition()[d] >= 0 && (*it_particule).getPosition()[d] < this->longueurs_caracteristiques[d];
                    d++;
                }
                if (!dans_univers){
                    std::cerr << "Erreur, une particule est partie : " << (*it_particule).getPosition() << std::endl;
                    it_particule = this->cellules[i].erase(it_particule);
                }
                else {
                    int indice = this->indice_n_dimensions((*it_particule).getPosition());
                    if (indice == i){
                        // c'est bon on passe au suivant
                        it_particule++;
                    }
                    else {
                        this->cellules[indice].push_front(*it_particule); // on ajoute la particule à la bonne cellule
                        it_particule = this->cellules[i].erase(it_particule); // on supprime la particule de cette cellule
                    }
                }
            }
        }
    }

    /// @brief Affiche les position et vitesses des particules de l'univers
    /// @param os le flux de sortie
    /// @return le flux de sortie après affichage des position et vitesse
    std::ostream& afficher_position(std::ostream& os){
        parcoure_particules([&os](Particule<Dim>* part){os << part->getPosition() << ":" << part->getVitesse().norme_2() << ";";});
        return os;
    }

    /// @brief Donne le nombre de particules dans l'univers
    int nombre_particules(){
        int nb = 0;
        parcoure_particules([&nb](Particule<Dim>* part){nb++;});
        return nb;
    }

    /// @brief Effectue une iteration de l'algorithme de Störmer-Verlet et incrémente le temps
    /// @param dt La pas de temps de l'itération
    /// @param le temps total qui va être incrémenté
    /// @details Comme la force ne dépend que de la position, on peut incrémenter deux fois la position et éviter de la recalculer pour l'incrémentation de la position puis de la vitesse
    void stormer_verlet_iteration(double dt, double& t){
        t += dt;
        // Met à jour vitesse avec ancienne force
        this->parcoure_particules([dt](Particule<Dim>* part){part->setVitesse(part->getVitesse() + dt * 0.5 / part->getMasse() * part->getForce());});
        // Met à jour la position
        this->parcoure_particules([dt](Particule<Dim>* part){part->setPosition(part->getPosition() + dt*(part->getVitesse()));});
        // On remet les particules dans leurs cellules
        this->mettre_a_jour_cellules();
        // On calcule maintenant les nouvelles forces
        this->calcule_force_interaction();
        // Met à jour vitesse avec nouvelle force
        this->parcoure_particules([dt](Particule<Dim>* part){part->setVitesse(part->getVitesse() + dt * 0.5 / part->getMasse() * part->getForce());});
    }

    /// @brief Affichage de l'état de l'univers dans un flux
    /// @param os le flux de sortie
    /// @param u l'univers
    /// @return le flux de sortie après affichage de l'univers
    friend std::ostream& operator<<(std::ostream& os, const Univers& u) {
        std::array<int, Dim> indice_dimension = {};
        os << "(";
        for (int d = 0; d < Dim; d++){
            os << u.nombre_cellules_dimension[d] << ",";
        }
        os << ") ";
        os << "{\n";
        for (int i = 0; i < u.nombre_cellules; i++){
            if (!u.cellules[i].empty()){
                os << "(";
                for (int d = 0; d < Dim; d++){
                    os << indice_dimension[d] << ",";
                }
                os << ") ";
                os << "cellule(";
                for (int d = 0; d < Dim; d++){
                    double depart = u.longueurs_caracteristiques[d] * (double)indice_dimension[d] / u.nombre_cellules_dimension[d];
                    double fin = depart + u.rayon_coupure;
                    os << "(" << depart  << "," << fin << "),";
                }
                os << "):\n";
                
                for (auto& part : u.cellules[i]){
                    os << "  " << part << "\n";
                }
                os << "\n";
            }
            indice_dimension[0]++;
            int d = 0;
            while(d < Dim-1 && indice_dimension[d] == u.nombre_cellules_dimension[d]){
                indice_dimension[d] = 0.0;
                d++;
                indice_dimension[d]++;
            }
        }
        os << "}\n";
        return os;
    }


    /// @brief Affichage de l'état de l'univers dans un flux de fichier au format xml de vtk
    /// @param os le flux de sortie
    /// @param u l'univers
    /// @return le flux de sortie après affichage de l'univers
    /// @warning si il y a beaucoup de particules, il faut avoir ouvert le fichier en std::ios::binary pour de l'écriture en binaire
    /// @todo j'utilise des doubles et je marque que j'utilise des float32 alors que les doubles sont sur 64 bits
    std::ostream& exporter_xml(std::ostream& os) {
        int limite_particules_ascii = 1000;
        os << "<VTKFile type=”UnstructuredGrid” version=”0.1” byte_order=”LittleEndian”>\n\t<UnstructuredGrid>";
        os << "\t\t<Piece NumberOfPoints=”" << this->nombre_particules() << "” NumberOfCells=”" << this->nombre_cellules << "”>\n";
        if (this->nombre_particules() <= limite_particules_ascii){ // on écrit en ascii
            os << "\t\t\t<Points>\n";
            os << "\t\t\t\t<DataArray name=”Position” type=”Float32” NumberOfComponents=”" << Dim << "” format=”ascii”>\n";
            os << "\t\t\t\t\t";
            this->parcoure_particules([&os](Particule<Dim>* part){
                Vecteur<Dim> v = part.getPosition();
                for (int i = 0; i < Dim; i++){
                    os << v[i] << " ";
                }
            });
            os << "\n";
            os << "\t\t\t\t</DataArray>\n";
            os << "\t\t\t</Points>\n";

            os << "\t\t\t<PointData Vectors=”vector”>\n";
            os << "\t\t\t\t<DataArray name=”Velocity” type=”Float32” NumberOfComponents=”" << Dim << "” format=”ascii”>\n";
            os << "\t\t\t\t\t";
            this->parcoure_particules([&os](Particule<Dim>* part){
                Vecteur<Dim> v = part.getVitesse();
                for (int i = 0; i < Dim; i++){
                    os << v[i] << " ";
                }
            });
            os << "\n";
            os << "\t\t\t\t</DataArray>\n";

            os << "\t\t\t\t<DataArray name=”Masse” type=”Float32” format=”ascii”>\n";
            os << "\t\t\t\t\t";
            this->parcoure_particules([&os](Particule<Dim>* part){
                os << part.getMasse() << " ";
            });
            os << "\n";
            os << "\t\t\t\t</DataArray>\n";
            os << "\t\t\t</PointData>\n";
        }
        else {
            os << "\t\t\t<Points>\n";
            os << "\t\t\t\t<DataArray name=”Position” type=”Float32” NumberOfComponents=”" << Dim << "” format=”binary”>\n";
            os << "\t\t\t\t\t";
            this->parcoure_particules([&os](Particule<Dim>* part){
                Vecteur<Dim> v = part.getPosition();
                for (int i = 0; i < Dim; i++){
                    double val = v[i];
                    os.write(reinterpret_cast<const char*>(&val), sizeof(val));
                }
            });
            os << "\n";
            os << "\t\t\t\t</DataArray>\n";
            os << "\t\t\t</Points>\n";

            os << "\t\t\t<PointData Vectors=”vector”>\n";
            os << "\t\t\t\t<DataArray name=”Velocity” type=”Float32” NumberOfComponents=”" << Dim << "” format=”binary”>\n";
            os << "\t\t\t\t\t";
            this->parcoure_particules([&os](Particule<Dim>* part){
                Vecteur<Dim> v = part.getVitesse();
                for (int i = 0; i < Dim; i++){
                    double val = v[i];
                    os.write(reinterpret_cast<const char*>(&val), sizeof(val));
                }
            });
            os << "\n";
            os << "\t\t\t\t</DataArray>\n";

            os << "\t\t\t\t<DataArray name=”Masse” type=”Float32” format=”binary”>\n";
            os << "\t\t\t\t\t";
            this->parcoure_particules([&os](Particule<Dim>* part){
                double val = part.getMasse();
                os.write(reinterpret_cast<const char*>(&val), sizeof(val));
            });
            os << "\n";
            os << "\t\t\t\t</DataArray>\n";
            os << "\t\t\t</PointData>\n";
        }

        os << "\t\t\t<Cells>\n";
        os << "\t\t\t\t<DataArray type=”Int32” Name=”connectivity” format=”ascii”>\n";
        os << "\t\t\t\t</DataArray>\n";
        os << "\t\t\t\t<DataArray type=”Int32” Name=”offsets” format=”ascii”>\n";
        os << "\t\t\t\t</DataArray>\n";
        os << "\t\t\t\t<DataArray type=”UInt8” Name=”types” format=”ascii”>\n";
        os << "\t\t\t\t</DataArray>\n";
        os << "\t\t\t</Cells>\n";

        os << "\t\t</Piece>\n";
        os << "\t</UnstructuredGrid>\n";
        os << "</VTKFile>";
        return os;
    }
};

#endif