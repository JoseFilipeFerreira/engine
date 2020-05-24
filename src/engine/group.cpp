#include "engine/group.hpp"

#include "utils/point.hpp"
#include "utils/types.hpp"

Group::Group() {
    transformations = std::vector<Transform>();
    models = std::vector<Object<model_t>>();
    terrains = std::vector<Object<terrain_t>>();
    colour = Colour();
    subgroups = std::vector<Group>();
}

void Group::draw_group(
    float elapsed, bool DEBUG, GroupBuffer const& group_buffer) const {
    for (auto const& t : transformations) t.apply(DEBUG, elapsed);
    for (auto const& l : lights) l.on();
    for (auto const& m : models) {
        if (m.is_visible()) {
            m.draw(group_buffer, DEBUG);
        }
    }
    for (auto const& t : terrains) {
        if (t.is_visible()) {
            t.draw(group_buffer, DEBUG);
        }
    }
    for (auto const& g : subgroups) {
        glPushMatrix();
        g.draw_group(elapsed, DEBUG, group_buffer);
        glPopMatrix();
    }
}
