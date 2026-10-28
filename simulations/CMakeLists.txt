# Simulations qui repondent aux questions des Labs 2 a 4.
add_executable(lab2_systeme_solaire lab2_systeme_solaire.cxx)  # Lab 2, Q4-Q6
target_link_libraries(lab2_systeme_solaire particules)

add_executable(lab3_univers_cube lab3_univers_cube.cxx)        # Lab 3, Q6-Q10
target_link_libraries(lab3_univers_cube particules)

add_executable(lab4_potentiel_lj lab4_potentiel_lj.cxx)        # Lab 4, Q1

add_executable(lab4_collision lab4_collision.cxx)              # Lab 4, Q6 + Lab 5, Q3
target_link_libraries(lab4_collision particules)
