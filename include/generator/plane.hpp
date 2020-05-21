#ifndef PLANE_H
#define PLANE_H
#include "utils/modelPoint.hpp"
#include "utils/point.hpp"

#include <vector>

class Plane {
  private:
    float side_length;

  public:
    Plane(int argc, char** args);
    std::vector<ModelPoint> draw() const;
};

#endif // PLANE_H
