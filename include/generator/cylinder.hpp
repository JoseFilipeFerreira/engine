#ifndef CYLINDER_H
#define CYLINDER_H
#include "utils/point.hpp"
#include "utils/types.hpp"

#include <vector>

class Cylinder {
  private:
    float _radius;
    float _height;
    u32 _slices;
    u32 _stacks;

  public:
    Cylinder(int argc, char** argv);
    std::vector<Point> draw() const;
};

#endif
