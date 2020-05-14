#include "generator/sphere.hpp"

#include "utils/point.hpp"

#include <cmath>

Sphere::Sphere(int argc, char** argv) {
    _radius = std::stod(argv[0]);
    _slices = std::stoi(argv[1]);
    _stacks = std::stoi(argv[2]);
}

std::vector<ModelPoint> Sphere::draw() const {
    std::vector<ModelPoint> coords;
    float a_slice = 2.0f * M_PI / _slices;
    float a_stack = M_PI / _stacks;

    for (i32 slice = 0; slice < _slices; slice++) {
        float curr_t_x = slice / static_cast<float>(_slices);
        float next_t_x = (slice + 1) / static_cast<float>(_slices);

        for (i32 stack = 0; stack < _stacks; stack++) {
            float curr_t_y = stack / static_cast<float>(_stacks);
            float next_t_y = (stack + 1) / static_cast<float>(_stacks);

            auto p0 = PointSpherical(_radius, a_stack * stack, a_slice * slice);
            auto p1 = p0.add_inclination(a_stack);
            auto p2 = p0.add_azimuth(a_slice);
            auto p3 = p2.add_inclination(a_stack);

            auto v0 = p0.normalized_vector();
            auto v1 = p1.normalized_vector();
            auto v2 = p2.normalized_vector();
            auto v3 = p3.normalized_vector();

            if (stack != 0) {
                // 1st triangle
                coords.emplace_back(p2, v2, next_t_x, curr_t_y);
                coords.emplace_back(p0, v0, curr_t_x, curr_t_y);
                coords.emplace_back(p3, v3, next_t_x, next_t_y);
            }
            if (stack != _stacks - 1) {
                // 2nd triangle
                coords.emplace_back(p3, v3, next_t_x, next_t_y);
                coords.emplace_back(p0, v0, curr_t_x, curr_t_y);
                coords.emplace_back(p1, v1, curr_t_x, next_t_y);
            }
        }
    }
    return coords;
}
