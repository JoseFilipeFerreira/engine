#ifndef CONE_H
#define CONE_H
#include "../utils/point.hpp"

#include <vector>

class Cone {
private:
    float _radius;
    int _slices;
    int _stacks;

public:
    Cone(int argc, char** argv);
    std::vector<Point> draw() const;
};

#endif // PLANE_H
