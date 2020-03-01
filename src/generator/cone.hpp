#ifndef CONE_H
#define CONE_H
#include "../utils/point.hpp"
#include "../utils/types.hpp"

#include <vector>

class Cone {
private:
    float _radius;
    u32 _slices;
    u32 _stacks;

public:
    Cone(int argc, char** argv);
    std::vector<Point> draw() const;
};

#endif // PLANE_H
