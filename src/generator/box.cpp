#include "generator/box.hpp"

#include "utils/point.hpp"

#include <stdexcept>

Box::Box(int argc, char** argv) {
    _x = std::stof(argv[0]);
    _y = std::stof(argv[1]);
    _z = std::stof(argv[2]);
    if (argc < 4)
        _slices = 1;
    else
        _slices = std::stoi(argv[3]) + 1;
}

std::vector<Point> Box::draw() const {
    std::vector<Point> coords;

    float stepx = _x / _slices;
    float stepy = _y / _slices;
    float stepz = _z / _slices;

    auto xy = Vector(stepx, stepy,     0);
    auto yz = Vector(0    , stepy, stepz);
    auto xz = Vector(stepx,     0, stepz);

    auto front  = Point(-_x/2, -_y/2,  _z/2);
    auto back   = Point( _x/2,  _y/2, -_z/2);

    for(i32 i = 0; i < _slices; i++) {
        for(i32 j = 0; j < _slices; j++) {
            //front
            coords.push_back(front + xy.hadamard(i  , j  , 0));
            coords.push_back(front + xy.hadamard(i+1, j  , 0));
            coords.push_back(front + xy.hadamard(i+1, j+1, 0));
            coords.push_back(front + xy.hadamard(i  , j  , 0));
            coords.push_back(front + xy.hadamard(i+1, j+1, 0));
            coords.push_back(front + xy.hadamard(i  , j+1, 0));
            //back
            coords.push_back(back + xy.hadamard(-i-1, -j-1, 0));
            coords.push_back(back + xy.hadamard(-i-1, -j  , 0));
            coords.push_back(back + xy.hadamard(-i  , -j  , 0));
            coords.push_back(back + xy.hadamard(-i  , -j-1, 0));
            coords.push_back(back + xy.hadamard(-i-1, -j-1, 0));
            coords.push_back(back + xy.hadamard(-i  , -j  , 0));
            //left
            coords.push_back(front + yz.hadamard(0, i  , -j  ));
            coords.push_back(front + yz.hadamard(0, i+1, -j  ));
            coords.push_back(front + yz.hadamard(0, i+1, -j-1));
            coords.push_back(front + yz.hadamard(0, i  , -j  ));
            coords.push_back(front + yz.hadamard(0, i+1, -j-1));
            coords.push_back(front + yz.hadamard(0, i  , -j-1));
            //right
            coords.push_back(back + yz.hadamard(0, -i-1, j+1));
            coords.push_back(back + yz.hadamard(0, -i-1, j  ));
            coords.push_back(back + yz.hadamard(0, -i  , j  ));
            coords.push_back(back + yz.hadamard(0, -i  , j+1));
            coords.push_back(back + yz.hadamard(0, -i-1, j+1));
            coords.push_back(back + yz.hadamard(0, -i  , j  ));
            //top
            coords.push_back(back + xz.hadamard(-i  , 0,   j));
            coords.push_back(back + xz.hadamard(-i-1, 0,   j));
            coords.push_back(back + xz.hadamard(-i-1, 0, j+1));
            coords.push_back(back + xz.hadamard(-i  , 0,   j));
            coords.push_back(back + xz.hadamard(-i-1, 0, j+1));
            coords.push_back(back + xz.hadamard(-i  , 0, j+1));
            //bottom
            coords.push_back(front + xz.hadamard(i+1, 0, -j-1));
            coords.push_back(front + xz.hadamard(i+1, 0,   -j));
            coords.push_back(front + xz.hadamard(i  , 0,   -j));
            coords.push_back(front + xz.hadamard(i  , 0, -j-1));
            coords.push_back(front + xz.hadamard(i+1, 0, -j-1));
            coords.push_back(front + xz.hadamard(i  , 0,   -j));
        }
    }
    return coords;
}
