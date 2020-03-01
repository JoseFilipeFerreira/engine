#include "box.hpp"
#include <stdexcept>

Box::Box(int argc, char** argv)
{
    _x = std::stof(argv[0]);
    _y = std::stof(argv[1]);
    _z = std::stof(argv[2]);
    if (argc < 4)
        _slices = 1;
    else
        _slices = std::stoi(argv[3]);
}

std::vector<Point> Box::draw() const
{
    std::vector<Point> coords;

    float stepx = _x / _slices;
    float stepy = _y / _slices;
    float stepz = _z / _slices;

    //front left bottom corner
    Point front  = Point(-_x/2, -_y/2,  _z/2);
    //back right top corner
    Point back   = Point(_x/2, _y/2, -_z/2);

    for(int i = 0; i < _slices; i++) {
        for(int j = 0; j < _slices; j++) {
            //front
            coords.push_back(front + Point(i*stepx    , j*stepy    , 0));
            coords.push_back(front + Point((i+1)*stepx, j*stepy    , 0));
            coords.push_back(front + Point((i+1)*stepx, (j+1)*stepy, 0));
            coords.push_back(front + Point(i*stepx    , j*stepy    , 0));
            coords.push_back(front + Point((i+1)*stepx, (j+1)*stepy, 0));
            coords.push_back(front + Point(i*stepx    , (j+1)*stepy, 0));
            //back
            coords.push_back(back + Point(-(i+1)*stepx, -(j+1)*stepy, 0));
            coords.push_back(back + Point(-(i+1)*stepx, -j*stepy    , 0));
            coords.push_back(back + Point(-i*stepx    , -j*stepy    , 0));
            coords.push_back(back + Point(-i*stepx    , -(j+1)*stepy, 0));
            coords.push_back(back + Point(-(i+1)*stepx, -(j+1)*stepy, 0));
            coords.push_back(back + Point(-i*stepx    , -j*stepy    , 0));
            //left
            coords.push_back(front + Point(0, i*stepy    , -j*stepz));
            coords.push_back(front + Point(0, (i+1)*stepy, -j*stepz));
            coords.push_back(front + Point(0, (i+1)*stepy, -(j+1)*stepz));
            coords.push_back(front + Point(0, i*stepy    , -j*stepz));
            coords.push_back(front + Point(0, (i+1)*stepy, -(j+1)*stepz));
            coords.push_back(front + Point(0, i*stepy    , -(j+1)*stepz));
            //right
            coords.push_back(back + Point(0, -(i+1)*stepy, (j+1)*stepz));
            coords.push_back(back + Point(0, -(i+1)*stepy, j*stepz));
            coords.push_back(back + Point(0, -i*stepy    , j*stepz));
            coords.push_back(back + Point(0, -i*stepy    , (j+1)*stepz));
            coords.push_back(back + Point(0, -(i+1)*stepy, (j+1)*stepz));
            coords.push_back(back + Point(0, -i*stepy    , j*stepz));
            //top
            coords.push_back(back + Point(-i*stepx    , 0, j*stepz));
            coords.push_back(back + Point(-(i+1)*stepx, 0, j*stepz));
            coords.push_back(back + Point(-(i+1)*stepx, 0, (j+1)*stepz));
            coords.push_back(back + Point(-i*stepx    , 0, j*stepz));
            coords.push_back(back + Point(-(i+1)*stepx, 0, (j+1)*stepz));
            coords.push_back(back + Point(-i*stepx    , 0, (j+1)*stepz));
            //bottom
            coords.push_back(front + Point(i*stepx    , 0, -j*stepz));
            coords.push_back(front + Point((i+1)*stepx, 0, -j*stepz));
            coords.push_back(front + Point((i+1)*stepx, 0, -(j+1)*stepz));
            coords.push_back(front + Point(i*stepx    , 0, -j*stepz));
            coords.push_back(front + Point((i+1)*stepx, 0, -(j+1)*stepz));
            coords.push_back(front + Point(i*stepx    , 0, -(j+1)*stepz));
        }
    }
    return coords;
}
