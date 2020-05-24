#ifndef GROUP_H
#define GROUP_H
#include "engine/model.hpp"
#include "engine/light.hpp"
#include "engine/buffers.hpp"

#include <vector>

class Group {
  private:
    std::vector<Transform> transformations;
    std::vector<Object<model_t>> models;
    std::vector<Object<terrain_t>> terrains;
    std::vector<Light> lights;
    Colour colour;
    std::vector<Group> subgroups;

  public:
    Group();
    Group(
        std::vector<Transform> t,
        std::vector<Object<model_t>> m,
        std::vector<Object<terrain_t>> ter,
        std::vector<Light> li,
        Colour c,
        std::vector<Group> g)
        : transformations(t), models(m), terrains(ter), lights(li), colour(c), subgroups(g) {}
    Group(std::vector<Group> g): subgroups(g) {}
    void draw_group(float, bool, GroupBuffer const&) const;
};

#endif // GROUP_H
