#ifndef CYLINDER_H
#define CYLINDER_H
#include "utils/modelPoint.hpp"
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
    std::vector<ModelPoint> draw() const;
};

#endif
