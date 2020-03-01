#include "sphere.hpp"

#define _USE_MATH_DEFINES
#include <iostream>
#include <stdexcept>
#include <cmath>

Sphere::Sphere(int argc, char** argv) {
    _radius = std::stod(argv[0]);
    _slices = std::stoi(argv[1]);
    _stacks = std::stoi(argv[2]);
}

std::vector<Point> Sphere::draw() const {
    std::vector<Point> coords;

    float ang_slice = 2 * M_PI / _slices;
    float ang_stack = M_PI / _stacks;

    for(i32 slice = 0; slice < _slices; slice++)
        for(i32 stack = 0; stack < _stacks; stack++)
        {
            coords.push_back(PointSpherical(_radius, ang_stack * (stack+1), ang_slice * (slice+1)));
            coords.push_back(PointSpherical(_radius, ang_stack * stack    , ang_slice * (slice+1)));
            coords.push_back(PointSpherical(_radius, ang_stack * stack    , ang_slice * slice));
            coords.push_back(PointSpherical(_radius, ang_stack * (stack+1), ang_slice * slice));
            coords.push_back(PointSpherical(_radius, ang_stack * (stack+1), ang_slice * (slice+1)));
            coords.push_back(PointSpherical(_radius, ang_stack * stack    , ang_slice * slice));
        }

    return coords;
}
