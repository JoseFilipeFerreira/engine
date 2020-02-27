#include "point.hpp"
#include <sstream>

using namespace std;

Point::Point(float x, float y, float z)
    : _x(x)
    , _y(y)
    , _z(z)
{
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
