#ifndef LIGHT_H
#define LIGHT_H

#include "engine/transformations.hpp"
#include "utils/point.hpp"
#include "utils/visitor.hpp"

#include <utility>
#include <variant>

class PointLight {
  private:
    Point _position;
    Colour _colour;
    u64 _id;

  public:
    PointLight(Point, Colour);
    void on() const;
};

class DirectionalLight {
  private:
    Vector _direction;
    Colour _colour;
    u64 _id;

  public:
    DirectionalLight(Vector, Colour);
    void on() const;
};

class SpotLight {
  private:
    Point _position;
    Vector _direction;
    Colour _colour;
    u64 _id;

  public:
    SpotLight(Point, Vector, Colour);
    void on() const;
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
