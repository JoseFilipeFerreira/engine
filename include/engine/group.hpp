#ifndef GROUP_H
#define GROUP_H
#include "engine/model.hpp"
#include "engine/light.hpp"
#include "engine/buffers.hpp"

#include <vector>

class Group {
  private:
    std::vector<Transform> transformations;
    std::vector<Object<true>> models;
    std::vector<Object<false>> terrains;
    std::vector<Light> lights;
    Colour colour;
    std::vector<Group> subgroups;

  public:
    Group();
    Group(
        std::vector<Transform> t,
        std::vector<Object<true>> m,
        std::vector<Object<false>> ter,
        std::vector<Light> li,
        Colour c,
        std::vector<Group> g)
        : transformations(t), models(m), terrains(ter), lights(li), colour(c), subgroups(g) {}
    Group(std::vector<Group> g): subgroups(g) {}
    void draw_group(float, bool, GroupBuffer const&) const;
};

#endif // GROUP_H
