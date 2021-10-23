cmake_minimum_required(VERSION 3.0.0)
project(Anter_Termite_Mayhem VERSION 0.1.0)

include(CTest)
enable_testing()

add_library(Anter_Termite_Mayhem Anter_Termite_Mayhem.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
