# Créer un executable à partir de fichiers sources
#add_executable(test_1  test_1.cxx)

# Fait le lien avec la bibliothèque créée précedemment
# Les entêtes sont automatiquement détectés.
#target_link_libraries(test_1 univers)

cmake_minimum_required(VERSION 3.20)
project(mwe)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG     v1.17.0
)
FetchContent_MakeAvailable(googletest)

# Apple Clang's default search list puts /usr/local/include before any -isystem
# directory. googletest marks its headers as SYSTEM, so a newer Homebrew gtest
# in /usr/local wins over the fetched sources and breaks the build (mixed API).
if(APPLE)
    foreach(_gtest_target IN ITEMS gtest gtest_main gmock gmock_main)
        if(TARGET ${_gtest_target})
        set_property(TARGET ${_gtest_target} PROPERTY
            INTERFACE_SYSTEM_INCLUDE_DIRECTORIES ””)
        endif()
    endforeach()
endif()

enable_testing()

add_executable(tests test.cxx)
target_link_libraries(tests GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(tests)