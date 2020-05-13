#ifndef CONE_H
#define CONE_H
#include "utils/modelPoint.hpp"
#include "utils/types.hpp"

#include <vector>

class Cone {
  private:
    float _radius;
    float _height;
    u32 _slices;
    u32 _stacks;

  public:
    Cone(int argc, char** argv);
    std::vector<ModelPoint> draw() const;
};

#endif // PLANE_H
