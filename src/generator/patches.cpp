#include "generator/patches.hpp"

#include "utils/slices.hpp"

#include <fstream>
#include <iostream>
#include <stdexcept>

Patches::Patches(int argc, char** argv) {
    _tesselation_level = std::stoi(argv[1]);
    std::ifstream file(argv[0]);

    file >> _n_patches;

    for (auto k = 0; k < _n_patches; k++) {
        std::vector<u64> patch;
        for (u64 i; file >> i;) {
            patch.push_back(i);
            if (file.peek() == ',' || file.peek() == ' ') file.ignore();
            if (file.peek() == '\n') break;
        }
        _index_patches.push_back(patch);
    }

    file >> _n_control_points;

    for (auto k = 0; k < _n_control_points; k++) {
        std::vector<float> point;
        for (float i; file >> i;) {
            point.push_back(i);
            if (file.peek() == ',' || file.peek() == ' ') file.ignore();
            if (file.peek() == '\n') break;
        }

        if (point.size() != 3) throw std::invalid_argument("Invalid Point");

        _control_points.push_back(Point(point[0], point[1], point[2]));
    }
}

// bezier matrix
static const float M[4][4] = {
    {-1.0f, +3.0f, -3.0f, +1.0f},
    {+3.0f, -6.0f, +3.0f, +0.0f},
    {-3.0f, +3.0f, +0.0f, +0.0f},
    {+1.0f, +0.0f, +0.0f, +0.0f}};

#define T(t) \
    { t *t *t, t *t, t, 1.0f }

#define dT(t) \
    { 3.0f * t *t, 2.0f * t, 1.0f, 0.0f }

void mult_vec_matrix4(const float v[4], const float m[4][4], float res[4]) {
    for (i64 i = 0; i < 4; i++) {
        res[i] = 0;
        for (i64 j = 0; j < 4; j++) {
            res[i] += v[j] * m[i][j];
        }
    }
}

auto tangent(
    SliceMatrix<Vector, 4> const& vectors, const float u[4], const float v[4])
    -> Vector {
    float u_m[4];
    mult_vec_matrix4(u, M, u_m);

    float v_m[4];
    mult_vec_matrix4(v, M, v_m);

    Vector ump[4];
    for (size_t i = 0; i < 4; i++)
        for (size_t j = 0; j < 4; j++)
            ump[i] = ump[i] + vectors[j][i] * u_m[j];

    Vector r;
    for (size_t i = 0; i < 4; i++)
        r = r + ump[i] * v_m[i];

    return r;
}

auto normal_generator(SliceMatrix<Vector, 4> const& vectors, float du, float dv)
    -> Vector {
    const float u[4] = T(du);
    const float d_u[4] = dT(du);
    const float v[4] = T(dv);
    const float d_v[4] = dT(dv);
    Vector tu = tangent(vectors, d_u, v);
    Vector tv = tangent(vectors, u, d_v);
    return tv.cross(tu).normalize();
}

auto point_generator(SliceMatrix<Vector, 4> const& vectors, float du, float dv)
    -> Point {
    const float u[4] = T(du);
    float u_m[4];
    mult_vec_matrix4(u, M, u_m);
    const float v[4] = T(dv);
    float v_m[4];
    mult_vec_matrix4(v, M, v_m);

    Vector ump[4];
    for (size_t i = 0; i < 4; i++)
        for (size_t j = 0; j < 4; j++)
            ump[i] = ump[i] + vectors[j][i] * u_m[j];

    Point r;
    for (size_t i = 0; i < 4; i++)
        r = r + ump[i] * v_m[i];

    return r;
}

auto Patches::patch_generator(u64 patch, float u, float v) const
    -> std::pair<Point, Vector> {
    std::vector<Vector> v_points;
    for (auto const& index : _index_patches[patch])
        v_points.push_back(_control_points[index]);

    auto point_matrix = SliceMatrix<Vector, 4>{v_points};

    return {
        point_generator(point_matrix, u, v),
        normal_generator(point_matrix, u, v)};
}

std::vector<ModelPoint> Patches::draw() const {
    std::vector<ModelPoint> coords;
    const float delta = 1.0f / _tesselation_level;

    for (i64 p = 0; p < _n_patches; p++) {
        for (i64 ui = 0; ui < _tesselation_level; ui++) {
            const float uf = ui * delta;
            const float next_uf = (ui + 1) * delta;

            float curr_t_x = ui / static_cast<float>(_tesselation_level);
            float next_t_x = (ui + 1) / static_cast<float>(_tesselation_level);

            for (i64 vi = 0; vi < _tesselation_level; vi++) {
                const float vf = vi * delta;
                const float next_vf = (vi + 1) * delta;

                float curr_t_y = vi / static_cast<float>(_tesselation_level);
                float next_t_y =
                    (vi + 1) / static_cast<float>(_tesselation_level);

                auto p0 =
                    ModelPoint(patch_generator(p, uf, vf), curr_t_x, curr_t_y);
                auto p1 = ModelPoint(
                    patch_generator(p, uf, next_vf), curr_t_x, next_t_y);
                auto p2 = ModelPoint(
                    patch_generator(p, next_uf, vf), next_t_x, curr_t_y);
                auto p3 = ModelPoint(
                    patch_generator(p, next_uf, next_vf), next_t_x, next_t_y);

                coords.push_back(p3);
                coords.push_back(p2);
                coords.push_back(p1);

                coords.push_back(p2);
                coords.push_back(p0);
                coords.push_back(p1);
            }
        }
    }

    return coords;
}
