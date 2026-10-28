# Créer une bibliothèque à partir d'un ensemble de fichiers de définitions (.cxx) sans main.
# Ces fichiers ne contiennent pas de fonction main
# Remplacer ... par les fichiers nécessaires
# Les classes du projet (Vector, Particule, Cellule, GrilleCellules, Univers,
# ExportateurVTK) sont entierement definies dans les entetes :
# pas de fichier .cxx a compiler, d'ou une bibliotheque INTERFACE qui se
# contente de propager le dossier include/.
add_library(particules INTERFACE)
target_include_directories(particules INTERFACE ${CMAKE_SOURCE_DIR}/include)
