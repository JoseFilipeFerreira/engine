#include "generator/cone.hpp"

#include "utils/point.hpp"

#include <cmath>

Cone::Cone(int argc, char** argv) {
    _radius = std::stof(argv[0]);
    _height = std::stof(argv[1]);
    _slices = std::stoi(argv[2]);
    _stacks = std::stoi(argv[3]);
}

std::vector<ModelPoint> Cone::draw() const {
    std::vector<ModelPoint> coords;

    float a_slice = 2.0f * M_PI / _slices;
    auto top_point = Point(0, _height, 0);

    auto b_normal = Vector(0, -1, 0);
    auto central = PointSpherical(0, 0, 0);

    auto normal_a = -std::tan(_height / _radius);

    for (i32 slice = 0; slice < _slices; slice++) {
        auto base = PointSpherical(_radius, M_PI_2, a_slice * slice);
        auto n_base = base.add_azimuth(a_slice);

        auto base_v = base.normalized_vector();
        auto n_base_v = n_base.normalized_vector();

        auto b_tex = Point(1.5, 0, 0.5) + base_v * 0.5f;
        auto n_b_tex = Point(1.5, 0, 0.5) + n_base_v * 0.5f;

        // base
        coords.emplace_back(n_base, b_normal, n_b_tex.x() / 2, n_b_tex.z());
        coords.emplace_back(base, b_normal, b_tex.x() / 2, b_tex.z());
        coords.emplace_back(central, b_normal, 1.5 / 2, 0.5);

        auto step = Vector(base, top_point) / _stacks;
        auto n_step = Vector(n_base, top_point) / _stacks;

        // side
        for (i32 stack = 0; stack < _stacks; stack++) {
            auto pivot = base + step * stack;
            auto n_pivot = n_base + n_step * stack;
            auto top = pivot + step;
            auto n_top = n_pivot + n_step;

            auto normal = VectorSpherical(1, normal_a, pivot.azimuth());
            auto n_normal = VectorSpherical(1, normal_a, n_pivot.azimuth());

            auto center_s = Point(0.5, 0, 0.5);

            auto f_stacks = static_cast<float>(_stacks);

            auto bs_tex = center_s + base_v * ((_stacks - stack) / f_stacks) * 0.5f;
            auto n_bs_tex = center_s + n_base_v * ((f_stacks - stack) / f_stacks) * 0.5f;

            auto ts_tex = center_s + base_v * ((f_stacks - stack - 1) / f_stacks) * 0.5f;
            auto n_ts_tex = center_s + n_base_v * ((f_stacks - stack - 1) / f_stacks) * 0.5f;

            if (!(stack == _stacks - 1)) {
                coords.emplace_back(pivot, normal, bs_tex.x() / 2, bs_tex.z());
                coords.emplace_back(n_top, n_normal, n_ts_tex.x() / 2, n_ts_tex.z());
                coords.emplace_back(top, normal, ts_tex.x() / 2, ts_tex.z());
            }

            coords.emplace_back(pivot, normal, bs_tex.x() / 2, bs_tex.z());
            coords.emplace_back(n_pivot, n_normal, n_bs_tex.x() / 2, n_bs_tex.z());
            coords.emplace_back(n_top, n_normal, n_ts_tex.x() / 2, n_ts_tex.z());
        }
    }
    return coords;
}
