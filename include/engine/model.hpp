#ifndef PLANE_H
#define PLANE_H

#include "utils/point.hpp"
#include <vector>

using namespace std;

class Model {
private:
    std::vector<Point> points;

public:
    Model(string);
    void draw_model();
};

class Models {
    private:
    std::vector<Model> models;

public:
    Models();
    Models(vector<string>);
    void draw_models();
};

#endif // PLANE_H
