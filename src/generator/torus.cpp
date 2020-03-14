#include "generator/torus.hpp"

#define _USE_MATH_DEFINES
#include <iostream>
#include <stdexcept>
#include <cmath>

Torus::Torus(int argc, char** argv) {
    _radius = std::stod(argv[0]);
    _ring_radius = std::stod(argv[0]);
    _slices       = std::stoi(argv[1]);
    _stacks       = std::stoi(argv[2]);
}

std::vector<Point> Torus::draw() const {
    std::vector<Point> coords;

    float ang_slice = 2 * M_PI / _slices;
    float ang_stack = M_PI / _stacks;

    for(i32 slice = 0; slice < _slices; slice++) {
        PointSpherical   center = PointSpherical(_radius, M_PI/2, ang_slice * slice);
        PointSpherical n_center = PointSpherical(_radius, M_PI/2, ang_slice * (slice+1));
        for(i32 stack = 0; stack < _stacks * 2; stack++)
        {
            VectorSpherical   border = VectorSpherical(_ring_radius, ang_stack * slice      ,   center.azimuth());
            VectorSpherical n_border = VectorSpherical(_ring_radius, ang_stack * (slice + 1), n_center.azimuth() );

            coords.push_back(  center +   border);
            coords.push_back(n_center +   border);
            coords.push_back(n_center + n_border);
            coords.push_back(  center +   border);
            coords.push_back(n_center + n_border);
            coords.push_back(  center + n_border);
        }
    }

    return coords;
}
