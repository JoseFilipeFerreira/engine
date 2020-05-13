#include "generator/plane.hpp"

Plane::Plane(int argc, char** args) {
    side_length = std::stof(args[0]);
}

std::vector<ModelPoint> Plane::draw() const {
    std::vector<ModelPoint> coords;
    float half = side_length / 2;

    auto v = Vector(0, 1, 0);

    auto p0 = Point(-half, 0, -half);
    auto p1 = Point( half, 0, -half);
    auto p2 = Point(-half, 0,  half);
    auto p3 = Point( half, 0,  half);

    //1st triangle
    coords.emplace_back(p0, v, 0, 0);
    coords.emplace_back(p2, v, 0, 1);
    coords.emplace_back(p1, v, 1, 0);

    //2nd triangle
    coords.emplace_back(p1, v, 1, 0);
    coords.emplace_back(p2, v, 0, 1);
    coords.emplace_back(p3, v, 1, 1);

    return coords;
}
