#include "engine/model.hpp"
#include "utils/types.hpp"
#include "deps/uniformrealdist.hpp"

#include <stdexcept>
#include <iostream>
#include <fstream>

#include <GL/gl.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

Model::Model(std::string fileName){
    float x, y, z;
    std::ifstream file(fileName.c_str());
    while (file >> x >> y >> z)
         points.push_back(Point(x, y, z));
}

void draw_triangle(Point p1, Point p2, Point p3){
    auto static const rng = UniformRealDist(0.0, 1.0);
    glColor3f(rng(), rng(), rng());
    glBegin(GL_TRIANGLES);
        glVertex3f(p1.x(), p1.y(), p1.z());
        glVertex3f(p2.x(), p2.y(), p2.z());
        glVertex3f(p3.x(), p3.y(), p3.z());
    glEnd();
}

void Model::draw_model() {
    for(i32 i = 0; i < points.size(); i += 3)
        draw_triangle(points[i], points[i+1], points[i+2]);
}

Models::Models(){
    models = std::vector<Model>();
}

Models::Models(std::vector<std::string> fileNames){
    for(std::string fileName: fileNames)
        models.push_back(Model(fileName));
}

void Models::draw_models() {
    for(Model m : models)
        m.draw_model();
}
