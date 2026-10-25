cmake_minimum_required(VERSION 3.16.3)
project(Lab4 CXX)
set(CMAKE_CXX_STANDARD 17)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release) # question6 simule 8000 particules sur 390000 pas : besoin de -O2
endif()

add_executable(question1 question1.cxx)

add_executable(question6 question6.cxx)
target_include_directories(question6 PRIVATE ../include)
