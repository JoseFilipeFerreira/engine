#include "model.hpp"
#include <stdexcept>
#include <iostream>
#include <fstream>

#include <GL/gl.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

using namespace std;

Model::Model(string fileName)
{
    float x, y, z;
    ifstream file(fileName.c_str());

    while (file >> x >> y >> z)
         points.push_back(Point(x, y, z));

    file.close();
}

void draw_triangle(Point p1, Point p2, Point p3){
    float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    glColor3f(r, g, b);
    glBegin(GL_TRIANGLES);
        glVertex3f(p1.x(), p1.y(), p1.z());
        glVertex3f(p2.x(), p2.y(), p2.z());
        glVertex3f(p3.x(), p3.y(), p3.z());
    glEnd();
}

void Model::draw_model() {
    for(int i = 0; i < points.size(); i += 3)
        draw_triangle(points[i], points[i+1], points[i+2]);
}

Models::Models(vector<string> fileNames){
    for(string fileName: fileNames)
        models.push_back(Model(fileName));
}

void Models::draw_models() {
    for(Model m : models)
        m.draw_model();
}
