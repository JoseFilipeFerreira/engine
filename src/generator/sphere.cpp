#define _USE_MATH_DEFINES

#include "sphere.hpp"
#include <stdexcept>
#include <math.h>

using namespace std;

Sphere::Sphere(int argc, char** argv)
{
    _radius = stod(argv[0]);
    _slices = stod(argv[1]);
    _stacks = stod(argv[2]);
}

std::vector<Point> Sphere::draw() const
{
    vector<Point> coords;

    float ang_slice = 2 * M_PI / _slices;
    float ang_stack = M_PI / _stacks;

    for(int slice = 0; slice < _slices; slice++)
        for(int stack = 0; stack < _stacks; stack++)
            coords.push_back(Point(Point_Spherical(_radius, ang_slice * slice, ang_stack * stack)));

    return coords;
}
