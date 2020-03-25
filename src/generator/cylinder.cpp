#include "generator/cylinder.hpp"
#include "utils/point.hpp"

#define _USE_MATH_DEFINES
#include <iostream>
#include <stdexcept>
#include <cmath>

Cylinder::Cylinder(int argc, char** argv) {
    _radius = std::stof(argv[0]);
    _height = std::stof(argv[1]);
    _slices = std::stoi(argv[2]);
    _stacks = std::stoi(argv[3]);
}

std::vector<Point> Cylinder::draw() const {
    std::vector<Point> coords;

    float ang_slice = 2 * M_PI / _slices;

    auto top = Vector(0, _height, 0);
    auto step = top / _stacks;

    for(i32 slice = 0; slice < _slices; slice++) {
        auto central = PointSpherical(0, 0, 0);
        auto base    = PointSpherical(_radius, M_PI/2, ang_slice * slice);
        auto n_base  = PointSpherical(_radius, M_PI/2, ang_slice *(slice+1));
        //top & bottom
        coords.push_back(n_base);
        coords.push_back(base);
        coords.push_back(central);
        coords.push_back(central + top);
        coords.push_back(base + top);
        coords.push_back(n_base + top);

        //side
        for(i32 stack = 0; stack < _stacks; stack++) {
            auto pivot   =   base + step * stack;
            auto n_pivot = n_base + step * stack;

            coords.push_back(pivot);
            coords.push_back(n_pivot + step);
            coords.push_back(pivot + step);
            coords.push_back(pivot);
            coords.push_back(n_pivot);
            coords.push_back(n_pivot + step);
        }
    }

    return coords;
}
