#include "cone.hpp"
#include <stdexcept>

using namespace std;

Cone::Cone(int argc, char** argv)
{
    _radius = stod(argv[0]);
    _slices = stod(argv[1]);
    _stacks = stod(argv[2]);
}

std::vector<Point> Cone::draw() const
{
    vector<Point> coords;

    return coords;
}
