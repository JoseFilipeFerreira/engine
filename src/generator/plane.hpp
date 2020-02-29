#ifndef PLANE_H
#define PLANE_H
#include <vector>
#include "../utils/point.hpp"

class Plane {
private:
    double side_length;

public:
    Plane(int argc, char** args);
    std::vector<Point> draw() const;
};

#endif // PLANE_H
