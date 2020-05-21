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

    auto patch_generator(u64, float, float) const -> std::pair<Point, Vector>;

  public:
    Patches(int argc, char** argv);
    std::vector<ModelPoint> draw() const;
};

#endif // PATCHES_H
