#include "generator/cylinder.hpp"

#include "utils/point.hpp"

#include <cmath>

Cylinder::Cylinder(int argc, char** argv) {
    _radius = std::stof(argv[0]);
    _height = std::stof(argv[1]);
    _slices = std::stoi(argv[2]);
    _stacks = std::stoi(argv[3]);
}

std::vector<ModelPoint> Cylinder::draw() const {
    std::vector<ModelPoint> coords;

    float a_slice = 2.0f * M_PI / _slices;

    auto top = Vector(0, _height, 0);
    auto step = top / _stacks;

    auto top_normal = Vector(0, 1, 0);
    auto bottom_normal = Vector(0, -1, 0);
    auto central = PointSpherical(0, 0, 0);

    auto b_center = Point(0.75, 0, 0.25);
    auto t_center = Point(0.25, 0, 0.25);

    for (i32 slice = 0; slice < _slices; slice++) {
        auto base = PointSpherical(_radius, M_PI_2, a_slice * slice);
        auto n_base = base.add_azimuth(a_slice);

        Vector base_v = base.normalized_vector();
        Vector n_base_v = n_base.normalized_vector();

        auto   b_coor = b_center +   base_v.mirror_z() * 0.25f;
        auto n_b_coor = b_center + n_base_v.mirror_z() * 0.25f;

        auto   t_coor = t_center +   base_v * 0.25f;
        auto n_t_coor = t_center + n_base_v * 0.25f;

        // bottom
        coords.emplace_back( n_base, bottom_normal, n_b_coor.x(), n_b_coor.z());
        coords.emplace_back(   base, bottom_normal,   b_coor.x(),   b_coor.z());
        coords.emplace_back(central, bottom_normal, b_center.x(), b_center.z());

        // top
        coords.emplace_back(central + top, top_normal, t_center.x(), t_center.z());
        coords.emplace_back(base    + top, top_normal,   t_coor.x(),   t_coor.z());
        coords.emplace_back(n_base  + top, top_normal, n_t_coor.x(), n_t_coor.z());

        // side
        float curr_t_x = slice / static_cast<float>(_slices);
        float next_t_x = (slice + 1) / static_cast<float>(_slices);

        for (i32 stack = 0; stack < _stacks; stack++) {
            auto pivot = base + step * stack;
            auto n_pivot = n_base + step * stack;

            float curr_t_y = 1 - stack / static_cast<float>(_stacks) * 0.5f;
            float next_t_y = 1 - (stack + 1) / static_cast<float>(_stacks) * 0.5f;

            // 1st triangle
            coords.emplace_back(pivot, base_v, curr_t_x, curr_t_y);
            coords.emplace_back(n_pivot + step, n_base_v, next_t_x, next_t_y);
            coords.emplace_back(pivot + step, base_v, curr_t_x, next_t_y);

            // 2nd triangle
            coords.emplace_back(pivot, base_v, curr_t_x, curr_t_y);
            coords.emplace_back(n_pivot, n_base_v, next_t_x, curr_t_y);
            coords.emplace_back(n_pivot + step, n_base_v, next_t_x, next_t_y);
        }
    }
    return coords;
}
