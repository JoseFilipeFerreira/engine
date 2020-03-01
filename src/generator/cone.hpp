#ifndef CONE_H
#define CONE_H
#include "primitives.hpp"

class Cone : public Primitive
{
private:
    float _radius;
    int _slices;
    int _stacks;

public:
    Cone(int argc, char** argv);
    std::vector<Point> draw() const;
};

#endif // PLANE_H
