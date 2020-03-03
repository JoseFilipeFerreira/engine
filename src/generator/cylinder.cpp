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

    Vector top = Vector(0, _height, 0);
    float ang_slice = 2 * M_PI / _slices;

    //top & bottom
    for(i32 slice = 0; slice < _slices; slice++) {
        PointSpherical p1 = PointSpherical(0, 0, 0);
        PointSpherical p2 = PointSpherical(_radius, M_PI/2, ang_slice * slice);
        PointSpherical p3 = PointSpherical(_radius, M_PI/2, ang_slice *(slice+1));
        coords.push_back(p3);
        coords.push_back(p2);
        coords.push_back(p1);
        coords.push_back(p1 + top);
        coords.push_back(p2 + top);
        coords.push_back(p3 + top);
    }

    //side
    Vector step = top / _stacks;
    for(i32 slice = 0; slice < _slices; slice++) {
        Point base   = PointSpherical(_radius, M_PI/2, ang_slice *  slice);
        Point n_base = PointSpherical(_radius, M_PI/2, ang_slice * (slice+1));

        for(i32 stack = 0; stack < _stacks; stack++) {
            Point pivot   =   base + step * stack;
            Point n_pivot = n_base + step * stack;

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
