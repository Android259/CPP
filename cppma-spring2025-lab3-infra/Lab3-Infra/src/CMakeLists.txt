# Créer une bibliothèque à partir d'un ensemble de fichiers de définitions (.cxx) sans main.
# Ces fichiers ne contiennent pas de fonction main
# Remplacer ... par les fichiers nécessaires
# Vector, Particule et Univers sont entierement definis dans les entetes
# (classes header-only) : pas de fichier .cxx a compiler, d'ou une
# bibliotheque INTERFACE qui se contente de propager le dossier include/.
add_library(particules INTERFACE)
target_include_directories(particules INTERFACE ${CMAKE_SOURCE_DIR}/include)
