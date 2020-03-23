#include "engine/model.hpp"

//#include "deps/uniformrealdist.hpp"
#include "utils/types.hpp"

#include <fstream>

Model::Model(char* fileName, Colour c) {
    colour = c;
    float x, y, z;
    auto file = std::ifstream(fileName);
    while (file >> x >> y >> z) points.push_back(Point(x, y, z));
}

void draw_triangle(Point const& p1, Point const& p2, Point const& p3) {
    //auto static const rng = UniformRealDist(0.0, 1.0);
    //glColor3f(rng(), rng(), rng());
    glBegin(GL_TRIANGLES);
    glVertex3f(p1.x(), p1.y(), p1.z());
    glVertex3f(p2.x(), p2.y(), p2.z());
    glVertex3f(p3.x(), p3.y(), p3.z());
    glEnd();
}

void Model::draw_model() const {
    colour.apply();
    for (i32 i = 0; i < points.size(); i += 3)
        draw_triangle(points[i], points[i + 1], points[i + 2]);
}

