#include "cone.hpp"

#include <stdexcept>

Cone::Cone(int argc, char** argv) {
    _radius = std::stof(argv[0]);
    _slices = std::stoi(argv[1]);
    _stacks = std::stoi(argv[2]);
}

std::vector<Point> Cone::draw() const {
    std::vector<Point> coords;

    return coords;
}
