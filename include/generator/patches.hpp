#ifndef PATCHES_H
#define PATCHES_H
#include "utils/point.hpp"
#include "utils/modelPoint.hpp"
#include "utils/types.hpp"

#include <vector>
#include <utility>

class Patches {
  private:
    u64 _tesselation_level;
    u64 _n_patches;
    std::vector<std::vector<u64>> _index_patches;
    u64 _n_control_points;
    std::vector<Point> _control_points;
    auto point_at(u64, u64) const -> Point;
    auto point_generator(u64, float, float) const -> Point;
    auto normal_generator(u64, float, float) const -> Vector;
    auto patch_generator(u64, float, float) const -> std::pair<Point, Vector>;
    auto tangent(u64, const float u[4], const float v[4]) const -> Vector;

  public:
    Patches(int argc, char** argv);
    std::vector<ModelPoint> draw() const;
};

#endif // PATCHES_H
