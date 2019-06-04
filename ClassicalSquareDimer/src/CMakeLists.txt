cmake_minimum_required(VERSION 2.8)
project(classical_dimer CXX)
set (CMAKE_CXX_STANDARD 11)
add_executable(dimer.out DimerLattice2D.cpp classical_dimer.cpp)
