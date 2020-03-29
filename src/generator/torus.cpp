#include "generator/torus.hpp"

#define _USE_MATH_DEFINES
#include <iostream>
#include <stdexcept>
#include <cmath>

Torus::Torus(int argc, char** argv) {
    double inner_radius = std::stod(argv[0]);
    double outer_radius = std::stod(argv[1]);

    _ring_radius  = (outer_radius - inner_radius)/2.0f;
    _radius       = inner_radius + _ring_radius;
    _slices       = std::stoi(argv[2]);
    _stacks       = std::stoi(argv[3]);
}

std::vector<Point> Torus::draw() const {
    std::vector<Point> coords;

    float ang_slice = 2.0f * M_PI / _slices;
    float ang_stack = 2.0f * M_PI / _stacks;

    for(i32 slice = 0; slice < _slices; slice++) {
        auto   center = PointSpherical(_radius, M_PI/2.0f, ang_slice * slice);
        auto n_center = PointSpherical(_radius, M_PI/2.0f, ang_slice * (slice+1));

        for(i32 stack = 0; stack < _stacks; stack++) {
            auto   base = VectorSpherical(_ring_radius, ang_stack *  stack     ,   center.azimuth());
            auto   top  = VectorSpherical(_ring_radius, ang_stack * (stack + 1),   center.azimuth());
            auto n_base = VectorSpherical(_ring_radius, ang_stack *  stack     , n_center.azimuth() );
            auto n_top  = VectorSpherical(_ring_radius, ang_stack * (stack + 1), n_center.azimuth() );

            // 1st triangle
            coords.push_back(  center + top);
            coords.push_back(n_center + n_base);
            coords.push_back(  center + base);

            //2nd triangle
            coords.push_back(  center + top);
            coords.push_back(n_center + n_top);
            coords.push_back(n_center + n_base);
        }

    }

    return coords;
}
