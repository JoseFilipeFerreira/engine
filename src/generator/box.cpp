#include "generator/box.hpp"

#include "utils/point.hpp"

Box::Box(int argc, char** argv) {
    _x = std::stof(argv[0]);
    _y = std::stof(argv[1]);
    _z = std::stof(argv[2]);
    if (argc < 4)
        _slices = 1;
    else
        _slices = std::stoi(argv[3]) + 1;
}
void Box::draw_face(
    std::vector<ModelPoint>& coords,
    Point org,
    Point tex_org,
    Vector base,
    Vector top) const {
    auto normal = base.cross(top).normalize();

    for (i32 i = 0; i < _slices; i++) {
        float xt = static_cast<float>(i) / (_slices * 3);
        float n_xt = static_cast<float>(i + 1) / (_slices * 3);

        for (i32 j = 0; j < _slices; j++) {
            float yt = static_cast<float>(j) / (_slices * 2);
            float n_yt = static_cast<float>(j + 1) / (_slices * 2);

            auto p0 = org + base * i + top * j;
            auto p1 = org + base * i + top * (j + 1);
            auto p2 = org + base * (i + 1) + top * (j + 1);
            auto p3 = org + base * (i + 1) + top * j;

            auto mp0 = ModelPoint(p0, normal, tex_org.x() + xt, tex_org.y() - yt);
            auto mp1 = ModelPoint(p1, normal, tex_org.x() + xt, tex_org.y() - n_yt);
            auto mp2 = ModelPoint(p2, normal, tex_org.x() + n_xt, tex_org.y() - n_yt);
            auto mp3 = ModelPoint(p3, normal, tex_org.x() + n_xt, tex_org.y() - yt);

            // 1st triangle
            coords.push_back(mp0);
            coords.emplace_back(mp2);
            coords.emplace_back(mp1);
            // 2nd triangle
            coords.push_back(mp0);
            coords.emplace_back(mp3);
            coords.emplace_back(mp2);
        }
    }
}

std::vector<ModelPoint> Box::draw() const {
    std::vector<ModelPoint> coords;

    auto x = Vector(_x / _slices, 0, 0);
    auto y = Vector(0, _y / _slices, 0);
    auto z = Vector(0, 0, _z / _slices);
    auto mx = x.mirror_x();
    auto my = y.mirror_y();
    auto mz = z.mirror_z();

    auto corner = Point(_x / 2, _y / 2, _z / 2);
    auto top = corner.mirror_x();
    auto right = corner.mirror_y();
    auto front = right.mirror_x();
    auto back = right.mirror_z();
    auto left = front.mirror_z();

    auto t_left = Point(0, 0.5f, 0);
    auto t_right = Point(0, 1, 0);
    auto t_top = Point(1.0f / 3.0f, 0.5f, 0);
    auto t_bottom = Point(1.0f / 3.0f, 1, 0);
    auto t_front = Point(2.0f / 3.0f, 0.5f, 0);
    auto t_back = Point(2.0f / 3.0f, 1, 0);

    // front
    draw_face(coords, front, t_front, x, y);
    // back
    draw_face(coords, back, t_back, mx, y);
    // left
    draw_face(coords, left, t_left, z, y);
    // right
    draw_face(coords, right, t_right, mz, y);
    // top
    draw_face(coords, top, t_top, x, mz);
    // bottom
    draw_face(coords, right, t_bottom, mx, mz);
    return coords;
}
