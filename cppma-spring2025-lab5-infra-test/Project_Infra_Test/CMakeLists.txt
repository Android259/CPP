cmake_minimum_required(VERSION 3.14)
project(project)

set(CMAKE_BUILD_TYPE Release)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(src)
add_subdirectory(doc)
add_subdirectory(demo)

# GoogleTest requires at least C++11

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.8.0
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

enable_testing()
add_subdirectory(test)
