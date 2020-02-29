#include "point.hpp"
#include <sstream>
#include <math.h>

using namespace std;

Point::Point(float x, float y, float z)
    : _x(x)
    , _y(y)
    , _z(z)
{
}

Point::Point(Point_Spherical p)
{
    _x = p.radius() * sin(p.inclination()) * cos(p.azimuth());
    _y = p.radius() * sin(p.inclination()) * sin(p.azimuth());
    _z = p.radius() * cos(p.inclination()); 
}

string Point_Spherical::to_string() const
{
    ostringstream sstream;
    sstream
        << _radius << ' '
        << _inclination << ' '
        << _azimuth;
    return sstream.str();
}

string Point::to_string() const
{
    ostringstream sstream;
    sstream
        << _x << ' '
        << _y << ' '
        << _z;
    return sstream.str();
}
