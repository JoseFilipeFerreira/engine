#ifndef SPHERE_H
#define SPHERE_H
#include "utils/point.hpp"
#include "utils/types.hpp"

#include <vector>

class Sphere {
  private:
    float _radius;
    u32 _slices;
    u32 _stacks;

  public:
    Sphere(int argc, char** argv);
    std::vector<Point> draw() const;
};

#endif // PLANE_H
