#ifndef LIGHT_H
#define LIGHT_H

#include "engine/transformations.hpp"
#include "utils/point.hpp"
#include "utils/visitor.hpp"

#include <utility>
#include <variant>

#ifdef __APPLE__
#    include <GLUT/glut.h>
#else
#    include <GL/glew.h>
#    include <GL/glut.h>
#endif

class PointLight {
  private:
    Point _position;
    Colour _colour;

  public:
    PointLight(Point p, Colour c): _position(p), _colour(c){}
    void on(GLenum) const;
};

class DirectionalLight {
  private:
    Vector _direction;
    Colour _colour;

  public:
    DirectionalLight(Vector v, Colour c): _direction(v), _colour(c){}
    void on(GLenum) const;
};

class SpotLight {
  private:
    Point _position;
    Vector _direction;
    Colour _colour;

  public:
    SpotLight(Point p, Vector v, Colour c)
        : _position(p), _direction(v), _colour(c) {}
    void on(GLenum) const;
};

class Light {
  private:
    std::variant<PointLight, DirectionalLight, SpotLight> _l;

  public:
    Light(PointLight l): _l(l){};
    Light(DirectionalLight l): _l(l){};
    Light(SpotLight l): _l(l){};

    void on() const;
};

#endif // LIGHT_H
