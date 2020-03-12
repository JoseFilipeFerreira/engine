#include "engine/model.hpp"

#include "deps/tinyxml.hpp"
#include "deps/uniformrealdist.hpp"
#include "utils/types.hpp"

#include <GL/gl.h>
#include <fstream>
#include <iostream>
#include <stdexcept>
#ifdef __APPLE__
#    include <GLUT/glut.h>
#else
#    include <GL/glut.h>
#endif

Model::Model(std::string fileName) {
    float x, y, z;
    std::ifstream file(fileName.c_str());
    while (file >> x >> y >> z) points.push_back(Point(x, y, z));
}

void draw_triangle(Point p1, Point p2, Point p3) {
    auto static const rng = UniformRealDist(0.0, 1.0);
    glColor3f(rng(), rng(), rng());
    glBegin(GL_TRIANGLES);
    glVertex3f(p1.x(), p1.y(), p1.z());
    glVertex3f(p2.x(), p2.y(), p2.z());
    glVertex3f(p3.x(), p3.y(), p3.z());
    glEnd();
}

void Model::draw_model() const {
    for (i32 i = 0; i < points.size(); i += 3)
        draw_triangle(points[i], points[i + 1], points[i + 2]);
}

Group::Group() {
    transformations = std::vector<Transform>();
    models = std::vector<Model>();
    subgroups = std::vector<Group>();
}

Group Parser(TiXmlElement* root) {
    std::vector<Transform> vTran;
    std::vector<Model> vMod;
    std::vector<Group> vGroup;

    for (TiXmlElement* elem = root->FirstChildElement(); elem != NULL;
         elem = elem->NextSiblingElement()) {
        std::string type = elem->Value();

        if (type == "translate") {
            float x = std::stof(elem->Attribute("X") ?: "0");
            float y = std::stof(elem->Attribute("Y") ?: "0");
            float z = std::stof(elem->Attribute("Z") ?: "0");
            vTran.push_back(Translate(x, y, z));
        } else if (type == "rotate") {
            float ang = std::stof(elem->Attribute("angle") ?: "0");
            float x = std::stof(elem->Attribute("axisX") ?: "0");
            float y = std::stof(elem->Attribute("axisY") ?: "0");
            float z = std::stof(elem->Attribute("axisZ") ?: "0");
            vTran.push_back(Translate(x, y, z));
        } else if (type == "scale") {
            float x = std::stof(elem->Attribute("X") ?: "1");
            float y = std::stof(elem->Attribute("Y") ?: "1");
            float z = std::stof(elem->Attribute("Z") ?: "1");
            vTran.push_back(Scale(x, y, z));
        } else if (type == "model") {
            vMod.push_back(Model(elem->Attribute("file")));
        } else {
            vGroup.push_back(Parser(elem));
        }
    }
    return Group(vTran, vMod, vGroup);
}

Group::Group(std::string fileName) {
    TiXmlDocument doc(fileName);
    if (!doc.LoadFile()) {
        std::cerr << doc.ErrorDesc() << std::endl;
        return;
    }

    TiXmlElement* root = doc.FirstChildElement();
    if (root == NULL) {
        std::cerr << "Failed to load file: No root element." << std::endl;
        doc.Clear();
        return;
    }

    *this = Parser(doc.FirstChildElement());
}

void Group::draw_group() const {
    for (auto& t : transformations) t.apply();

    for (auto& m : models) m.draw_model();

    for (auto& g : subgroups) {
        glPushMatrix();
        g.draw_group();
        glPopMatrix();
    }
}
