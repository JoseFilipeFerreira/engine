#include "generator/torus.hpp"

#include "utils/point.hpp"

#include <cmath>

Torus::Torus(int argc, char** argv) {
    double inner_radius = std::stod(argv[0]);
    double outer_radius = std::stod(argv[1]);

    _ring_radius = (outer_radius - inner_radius) / 2.0f;
    _radius = inner_radius + _ring_radius;
    _slices = std::stoi(argv[2]);
    _stacks = std::stoi(argv[3]);
}

std::vector<ModelPoint> Torus::draw() const {
    std::vector<ModelPoint> coords;

    float a_slice = 2.0f * M_PI / _slices;
    float a_stack = 2.0f * M_PI / _stacks;

    for (i32 slice = 0; slice < _slices; slice++) {
        auto center = PointSpherical(_radius, M_PI_2, a_slice * slice);
        auto n_center = center.add_azimuth(a_slice);

        float curr_t_y = static_cast<float>(slice) / _slices;
        float next_t_y = static_cast<float>(slice + 1) / _slices;

        for (i32 stack = 0; stack < _stacks; stack++) {
            auto base = VectorSpherical(
                _ring_radius, a_stack * stack, center.azimuth());
            auto n_base = VectorSpherical(
                _ring_radius, a_stack * stack, n_center.azimuth());

            auto top = base.add_inclination(a_stack);
            auto n_top = n_base.add_inclination(a_stack);

            auto p0 = center + base;
            auto p1 = center + top;
            auto p2 = n_center + n_base;
            auto p3 = n_center + n_top;

            float base_t_x = static_cast<float>(stack) / _stacks;
            float top_t_x = static_cast<float>(stack + 1) / _stacks;

            // 1st triangle
            coords.emplace_back(p1, top.normalize(), top_t_x, 0);
            coords.emplace_back(p2, n_base.normalize(), base_t_x, 1);
            coords.emplace_back(p0, base.normalize(), base_t_x, 0);

            // 2nd triangle
            coords.emplace_back(p1, top.normalize(), top_t_x, 0);
            coords.emplace_back(p3, n_top.normalize(), top_t_x, 1);
            coords.emplace_back(p2, n_base.normalize(), base_t_x, 1);
        }
    }

    return coords;
}
