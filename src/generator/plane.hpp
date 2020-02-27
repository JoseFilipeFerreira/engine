#ifndef PLANE_H
#define PLANE_H
#include "primitives.hpp"

class Plane : public Primitive {
private:
    double side_length;

public:
    Plane(int argc, char** args);
    std::vector<Point> draw() const;
};

#endif // PLANE_H
