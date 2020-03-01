#ifndef SPHERE_H
#define SPHERE_H
#include "../utils/point.hpp"

#include <vector>

class Sphere {
private:
    float _radius;
    float _slices;
    float _stacks;

public:
    Sphere(int argc, char** argv);
    std::vector<Point> draw() const;
};

#endif // PLANE_H
