#include "generator/cone.hpp"
#include "utils/point.hpp"

#define _USE_MATH_DEFINES
#include <iostream>
#include <stdexcept>
#include <cmath>

Cone::Cone(int argc, char** argv) {
    _radius = std::stof(argv[0]);
    _height = std::stof(argv[1]);
    _slices = std::stoi(argv[2]);
    _stacks = std::stoi(argv[3]);
}

std::vector<Point> Cone::draw() const {
    std::vector<Point> coords;

    float ang_slice = 2 * M_PI / _slices;

    Point top_point = Point(0, _height, 0);

    for(i32 slice = 0; slice < _slices; slice++) {
        coords.push_back(PointSpherical(_radius, M_PI/2, ang_slice *(slice+1)));
        coords.push_back(PointSpherical(_radius, M_PI/2, ang_slice * slice));
        coords.push_back(PointSpherical(0, 0, 0));
    }

    for(i32 slice = 0; slice < _slices; slice++) {
        Point base   = PointSpherical(_radius, M_PI/2, ang_slice *  slice);
        Point n_base = PointSpherical(_radius, M_PI/2, ang_slice * (slice+1));

        Vector step = Vector(base, top_point) / _stacks;
        Vector n_step = Vector(n_base, top_point) / _stacks;

        for(i32 stack = 0; stack < _stacks; stack++) {
            Point pivot =  base + step * stack;
            Point n_pivot =  n_base + n_step * stack;

            if(!(stack == _stacks -1)) {
                coords.push_back(pivot);
                coords.push_back(n_pivot + n_step);
                coords.push_back(pivot + step);
            }
            coords.push_back(pivot);
            coords.push_back(n_pivot);
            coords.push_back(n_pivot + n_step);
        }
    }

    return coords;
}
