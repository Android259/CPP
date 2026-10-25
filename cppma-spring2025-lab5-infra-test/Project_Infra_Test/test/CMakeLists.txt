# Pour chaque test (famille de test), un exécutable est cré
add_executable(yyyy yyyy.cc)

## Ne pas oublier d'ajouter la bibliothèque du projet (xxxx)
target_link_libraries( hello_test gtest_main xxxx)

include(GoogleTest)
gtest_discover_tests(hello_test)