#ifndef MODEL_H
#define MODEL_H

#include "utils/point.hpp"

#include <GL/gl.h>
#include <variant>
#include <vector>
#ifdef __APPLE__
#    include <GLUT/glut.h>
#else
#    include <GL/glut.h>
#endif

template<class... Ts>
struct overloaded: Ts... {
    using Ts::operator()...;
};
template<class... Ts>
overloaded(Ts...)->overloaded<Ts...>; // not needed as of C++20

class Model {
  private:
    std::vector<Point> points;

  public:
    Model(std::string);
    void draw_model() const;
};

class Scale {
  private:
    float _x, _y, _z;

  public:
    Scale(float x, float y, float z): _x(x), _y(y), _z(z) {}
    void apply() const { glScalef(_x, _y, _z); }
};

class Colour {
  private:
    float _r, _g, _b, _a;

  public:
    Colour(float r, float g, float b, float a): _r(r), _g(g), _b(b), _a(a) {}
    void apply() const { glColor4f(_r, _g, _b, _a); }
};

class Translate {
  private:
    float _x, _y, _z;

  public:
    Translate(float x, float y, float z): _x(x), _y(y), _z(z) {}
    void apply() const { glTranslatef(_x, _y, _z); }
};

class Rotate {
  private:
    float _ang, _x, _y, _z;

  public:
    Rotate(float ang, float x, float y, float z)
        : _ang(ang), _x(x), _y(y), _z(z) {}
    void apply() const { glRotatef(_ang, _x, _y, _z); }
};

class Transform {
  private:
    std::variant<Scale, Translate, Rotate, Colour> _t;

  public:
    Transform(Rotate t): _t(t){};
    Transform(Scale t): _t(t){};
    Transform(Translate t): _t(t){};
    Transform(Colour t): _t(t){};
    void apply() const {
        std::visit(
            overloaded{
                [](Scale t) { t.apply(); },
                [](Translate t) { t.apply(); },
                [](Rotate t) { t.apply(); },
                [](Colour t) { t.apply(); },
            },
            _t);
    }
};

class Group {
  private:
    std::vector<Transform> transformations;
    std::vector<Model> models;
    std::vector<Group> subgroups;

  public:
    Group();
    Group(std::vector<Transform> t, std::vector<Model> m, std::vector<Group> g)
        : transformations(t), models(m), subgroups(g) {}
    Group(std::string);
    void draw_group() const;
};

#endif // MODEL_H
