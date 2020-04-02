#ifndef MODEL_H
#define MODEL_H

#include "utils/types.hpp"

#include <string>
#include <variant>
#include <vector>
#include <iostream>

#ifdef __APPLE__
#    include <GLUT/glut.h>
#else
#    include <GL/glew.h>
#    include <GL/glut.h>
#endif

template<class... Ts>
struct overloaded: Ts... {
    using Ts::operator()...;
};
template<class... Ts>
overloaded(Ts...)->overloaded<Ts...>; // not needed as of C++20

class Colour {
  private:
    float _r, _g, _b, _a;

  public:
    Colour(): _r(0), _g(0), _b(0), _a(1) {}
    Colour(float r, float g, float b, float a): _r(r), _g(g), _b(b), _a(a) {}
    void apply() const { glColor4f(_r, _g, _b, _a); }
};

class Scale {
  private:
    float _x, _y, _z;

  public:
    Scale(float x, float y, float z): _x(x), _y(y), _z(z) {}
    void apply(float elapsed) const { glScalef(_x, _y, _z); }
};

class Translate {
  private:
    float _x, _y, _z;

  public:
    Translate(float x, float y, float z): _x(x), _y(y), _z(z) {}
    void apply(float elapsed) const { glTranslatef(_x, _y, _z); }
};

class Rotate {
  private:
    float _ang, _x, _y, _z, _time;

  public:
    Rotate(float ang, float x, float y, float z, float time)
        : _ang(ang), _x(x), _y(y), _z(z), _time(time) {}
    void apply(float elapsed) const {
        float total_ang = _ang + elapsed * (_time?360.0f / _time:0);
        glRotatef(total_ang, _x, _y, _z);
    }
};

class Transform {
  private:
    std::variant<Scale, Translate, Rotate> _t;

  public:
    Transform(Rotate t): _t(t){};
    Transform(Scale t): _t(t){};
    Transform(Translate t): _t(t){};

    void apply(float elapsed) const {
        std::visit(
            overloaded{
                [&](Scale t) { t.apply(elapsed); },
                [&](Translate t) { t.apply(elapsed); },
                [&](Rotate t) { t.apply(elapsed); },
            },
            _t);
    }
};

class ModelBuffer {
  private:
    GLuint _buffers[1];
    size_t _n_vertices;

  public:
    ModelBuffer(std::string const&);
    void draw_model() const;
};

class Model {
  private:
    std::string _file_name;
    Colour _colour;

  public:
    Model(const char*, Colour);
    void apply_colour() const { _colour.apply(); };
    auto model_name() const -> std::string const& { return _file_name; }
};

#endif // MODEL_H
