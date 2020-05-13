#ifndef TORUS_H
#define TORUS_H
#include "utils/modelPoint.hpp"
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
    std::vector<ModelPoint> draw() const;
};

#endif // PLANE_H
