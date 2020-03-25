cmake_minimum_required(VERSION 3.5)

add_executable(generator
    main.cpp
    cone.cpp
    box.cpp
    plane.cpp
    sphere.cpp
    cylinder.cpp
    torus.cpp
)

target_link_libraries(
    generator
    utils
    deps
)

