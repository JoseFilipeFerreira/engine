#ifndef PLANE_H
#define PLANE_H

#include "utils/point.hpp"
#include <vector>

class Model {
private:
    std::vector<Point> points;

public:
    Model(std::string);
    void draw_model();
};

class Models {
    private:
    std::vector<Model> models;

public:
    Models();
    Models(std::vector<std::string>);
    void draw_models();
};

#endif // PLANE_H
