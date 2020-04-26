#include "generator/patches.hpp"

#include "utils/point.hpp"

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

auto bezier_point(
    float t, Point const p0, Point const p1, Point const p2, Point const p3)
    -> Point {
    const float m[4][4] = {
        {-1, +3, -3, +1}, {+3, -6, +3, +0}, {-3, +3, +0, +0}, {+1, +0, +0, +0}};

    const float pm[4][3] = {
        {p0.x(), p0.y(), p0.z()},
        {p1.x(), p1.y(), p1.z()},
        {p2.x(), p2.y(), p2.z()},
        {p3.x(), p3.y(), p3.z()}};

    float a[4][3] = {0};
    for (i32 i = 0; i < 4; i++)
        for (i32 j = 0; j < 3; j++)
            for (i32 k = 0; k < 4; k++) a[i][j] += m[i][k] * pm[k][j];

    const float tv[4] = {t * t * t, t * t, t, 1};

    float pv[3] = {0};
    for (i32 j = 0; j < 3; j++)
        for (i32 k = 0; k < 4; k++) pv[j] += tv[k] * a[k][j];

    return Point(pv[0], pv[1], pv[2]);
}

auto Patches::patch_generator(u64 patch, float u, float v) const -> Point {
    std::vector<Point> patch_control_points;
    for (auto const& index : _index_patches[patch]) {
        patch_control_points.push_back(_control_points[index]);
    }

    std::vector<Point> new_patch_points;
    for (auto i = patch_control_points.cbegin();
         i != patch_control_points.cend();
         i += 4) {
        new_patch_points.push_back(bezier_point(u, i[0], i[1], i[2], i[3]));
    }

    return bezier_point(
        v,
        new_patch_points[0],
        new_patch_points[1],
        new_patch_points[2],
        new_patch_points[3]);
}

std::vector<Point> Patches::draw() const {
    std::vector<Point> coords;
    const float delta = 1.0f / _tesselation_level;

    for (i64 p = 0; p < _n_patches; p++) {
        for (i64 ui = 0; ui < _tesselation_level; ui++) {
            const float uf = ui * delta;
            const float next_uf = (ui + 1) * delta;
            for (i64 vi = 0; vi < _tesselation_level; vi++) {
                const float vf = vi * delta;
                const float next_vf = (vi + 1) * delta;

                auto p0 = patch_generator(p, uf, vf);
                auto p1 = patch_generator(p, uf, next_vf);
                auto p2 = patch_generator(p, next_uf, vf);
                auto p3 = patch_generator(p, next_uf, next_vf);

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
