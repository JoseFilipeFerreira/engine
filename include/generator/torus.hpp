#ifndef TORUS_H
#define TORUS_H
#include "utils/point.hpp"
#include "utils/types.hpp"

#include <vector>

class Torus {
  private:
    float _radius;
    float _ring_radius;
    u32 _slices;
    u32 _stacks;

  public:
    Torus(int argc, char** argv);
    std::vector<Point> draw() const;
};

#endif // PLANE_H
