#ifndef EXPORTATEUR_VTK_HPP
#define EXPORTATEUR_VTK_HPP

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "Particule.hpp"

// ExportateurVTK : serialise une liste de particules au format VTK XML
// UnstructuredGrid (.vtu), visualisable avec Paraview ou Visit (Question 3,
// Visualisation). Classe utilitaire separee de Univers : la serialisation
// est une preoccupation d'infrastructure (E/S), pas de domaine physique
// (cf. Cours 05 - architecture en couches).
class ExportateurVTK {
public:
    template <std::size_t Dim>
    static void exporter(const std::vector<Particule<Dim>>& particules, const std::string& nomFichier) {
        std::ofstream fichier(nomFichier);

        fichier << "<VTKFile type=\"UnstructuredGrid\" version=\"0.1\" byte_order=\"LittleEndian\">\n";
        fichier << "<UnstructuredGrid>\n";
        fichier << "<Piece NumberOfPoints=\"" << particules.size() << "\" NumberOfCells=\"0\">\n";

        fichier << "<Points>\n";
        fichier << "<DataArray name=\"Position\" type=\"Float32\" NumberOfComponents=\"" << Dim << "\" format=\"ascii\">\n";
        for (const auto& p : particules) {
            for (std::size_t d = 0; d < Dim; ++d) {
                fichier << p.getPosition()[d] << " ";
            }
        }
        fichier << "\n</DataArray>\n";
        fichier << "</Points>\n";

        fichier << "<PointData Vectors=\"vector\">\n";
        fichier << "<DataArray type=\"Float32\" Name=\"Velocity\" NumberOfComponents=\"" << Dim << "\" format=\"ascii\">\n";
        for (const auto& p : particules) {
            for (std::size_t d = 0; d < Dim; ++d) {
                fichier << p.getVitesse()[d] << " ";
            }
        }
        fichier << "\n</DataArray>\n";
        fichier << "<DataArray type=\"Float32\" Name=\"Masse\" format=\"ascii\">\n";
        for (const auto& p : particules) {
            fichier << p.getMasse() << " ";
        }
        fichier << "\n</DataArray>\n";
        fichier << "</PointData>\n";

        fichier << "<Cells>\n";
        fichier << "<DataArray type=\"Int32\" Name=\"connectivity\" format=\"ascii\">\n";
        fichier << "</DataArray>\n";
        fichier << "<DataArray type=\"Int32\" Name=\"offsets\" format=\"ascii\">\n";
        fichier << "</DataArray>\n";
        fichier << "<DataArray type=\"UInt8\" Name=\"types\" format=\"ascii\">\n";
        fichier << "</DataArray>\n";
        fichier << "</Cells>\n";

        fichier << "</Piece>\n";
        fichier << "</UnstructuredGrid>\n";
        fichier << "</VTKFile>\n";
    }
};

#endif // EXPORTATEUR_VTK_HPP
