cmake_minimum_required(VERSION 3.5)

# Project Name
project(generator)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_executable(generator main.cpp plane.cpp box.cpp sphere.cpp cone.cpp ../utils/point.cpp ../utils/types.hpp) 
