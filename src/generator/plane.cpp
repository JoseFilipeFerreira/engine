#include "plane.hpp"
#include <stdexcept>

using namespace std;

Plane::Plane(int argc, char** args)
{
    side_length = stod(*args);
}

std::vector<Point> Plane::draw() const
{
    vector<Point> coords;
    float half = side_length / 2;

    Point p0 = Point(-half, 0, -half);
    Point p1 = Point( half, 0, -half);
    Point p2 = Point(-half, 0, half);
    Point p3 = Point( half, 0, half);

    //1st triangle
    coords.push_back(p0);
    coords.push_back(p2);
    coords.push_back(p1);

    //2nd triangle
    coords.push_back(p0);
    coords.push_back(p3);
    coords.push_back(p2);

    return coords;
}
