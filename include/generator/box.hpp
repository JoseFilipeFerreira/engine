#ifndef BOX_H
#define BOX_H
#include "utils/modelPoint.hpp"
#include "utils/types.hpp"

#include <vector>

class Box {
  private:
    float _x, _y, _z;
    u32 _slices;
    void draw_face(std::vector<ModelPoint>&, Point, Point, Vector, Vector) const;

  public:
    Box(int argc, char** argv);
    std::vector<ModelPoint> draw() const;
};

#endif // PLANE_H
