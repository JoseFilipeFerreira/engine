#include "box.hpp"
#include <stdexcept>

using namespace std;

Box::Box(int argc, char** argv)
{
    _x = stod(argv[0]);
    _y = stod(argv[1]);
    _z = stod(argv[2]);
    if (argc < 4)
        _slices = 1;
    else
        _slices = stoi(argv[3]);
}

std::vector<Point> Box::draw() const
{
    vector<Point> coords;

    return coords;
}
