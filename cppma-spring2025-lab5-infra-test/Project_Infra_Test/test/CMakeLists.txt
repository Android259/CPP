# Pour chaque test (famille de test), un exécutable est cré
add_executable(tests
    test_vector.cxx
    test_particule.cxx
    test_cellule.cxx
    test_univers.cxx
)

## Ne pas oublier d'ajouter la bibliothèque du projet (xxxx)
target_link_libraries(tests gtest_main particules)

include(GoogleTest)
gtest_discover_tests(tests)
