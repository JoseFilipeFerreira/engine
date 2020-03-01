#ifndef BOX_H
#define BOX_H
#include "../utils/point.hpp"
#include "../utils/types.hpp"

#include <vector>

class Box {
private:
    float _x;
    float _y;
    float _z;
    u32 _slices;

public:
    Box(int argc, char** argv);
    std::vector<Point> draw() const;
};

#endif // PLANE_H
