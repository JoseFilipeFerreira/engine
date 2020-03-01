#ifndef SPHERE_H
#define SPHERE_H
#include "primitives.hpp"

class Sphere : public Primitive
{
private:
    float _radius;
    float _slices;
    float _stacks;

public:
    Sphere(int argc, char** argv);
    std::vector<Point> draw() const;
};

#endif // PLANE_H
