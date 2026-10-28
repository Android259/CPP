# Lab 1 : exercices autonomes (pas de dependance a la bibliotheque de
# particules). -Wall -Wextra -g : avertissements et symboles de debogage, pour
# les outils de debogage et de profilage utilises pendant cette seance.
add_executable(lab1_trace trace.cxx)
target_compile_options(lab1_trace PRIVATE -Wall -Wextra -g)

add_executable(lab1_edo edo.cxx)
target_compile_options(lab1_edo PRIVATE -Wall -Wextra -g)
