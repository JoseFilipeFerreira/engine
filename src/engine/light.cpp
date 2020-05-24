#include "engine/light.hpp"

#include "utils/colour.hpp"
#include "utils/types.hpp"

#include <atomic>
#include <stdexcept>
#include <sstream>

#ifdef __APPLE__
#    include <GLUT/glut.h>
#else
#    include <GL/glew.h>
#    include <GL/glut.h>
#endif

auto gl_light(size_t number) -> GLenum {
    return (GLenum[]){
        GL_LIGHT0,
        GL_LIGHT1,
        GL_LIGHT2,
        GL_LIGHT3,
        GL_LIGHT4,
        GL_LIGHT5,
        GL_LIGHT6,
        GL_LIGHT7}[number];
}

auto get_new_id() -> u64 {
    static std::atomic<u64> id = 0;
    if (id == 8) {
        std::stringstream ss;
        ss << RED << "error: " << RESET << "More than 8 lights were defined";
        throw std::invalid_argument(ss.str());
    }
    return id++;
}

PointLight::PointLight(Point p, Colour c)
    : _position(p), _colour(c), _id(get_new_id()) {}

void PointLight::on() const {
    auto light = gl_light(_id);
    glEnable(light);

    float pos[4] = {_position.x(), _position.y(), _position.z(), 1.0f};

    auto const ambient = _colour * 0.2f;

    GLfloat amb[4] = {ambient.r(), ambient.g(), ambient.b(), 1.0f};
    GLfloat diff[4] = {_colour.r(), _colour.g(), _colour.b(), 1.0};
    glLightfv(light, GL_POSITION, pos);
    glLightfv(light, GL_AMBIENT, amb);
    glLightfv(light, GL_DIFFUSE, diff);

    GLfloat atten = 0.0f;
    glLightfv(light, GL_LINEAR_ATTENUATION, &atten);
}

DirectionalLight::DirectionalLight(Vector v, Colour c)
    : _direction(v), _colour(c), _id(get_new_id()) {}

void DirectionalLight::on() const {
    auto light = gl_light(_id);
    glEnable(light);

    auto const ambient = _colour * 0.2f;

    GLfloat amb[4] = {ambient.r(), ambient.g(), ambient.b(), 1.0f};
    GLfloat diff[4] = {_colour.r(), _colour.g(), _colour.b(), 1.0};
    GLfloat dir[4] = {_direction.x(), _direction.y(), _direction.z(), 0.0f};

    glLightfv(light, GL_POSITION, dir);
    glLightfv(light, GL_AMBIENT, amb);
    glLightfv(light, GL_DIFFUSE, diff);
}

SpotLight::SpotLight(Point p, Vector v, Colour c)
    : _position(p), _direction(v), _colour(c), _id(get_new_id()) {}

void SpotLight::on() const {
    auto light = gl_light(_id);
    glEnable(light);

    GLfloat diff[4] = {_colour.r(), _colour.g(), _colour.b(), 1.0f};

    GLfloat pos[4] = {_position.x(), _position.y(), _position.z(), 1.0f};
    GLfloat dir[3] = {_direction.x(), _direction.y(), _direction.z()};

    glLightfv(light, GL_POSITION, pos);
    glLightfv(light, GL_DIFFUSE, diff);
    glLightfv(light, GL_SPOT_DIRECTION, dir);
    glLightf(light, GL_SPOT_CUTOFF, 45.0);
    glLightf(light, GL_SPOT_EXPONENT, 0.0);
}

void Light::on() const {
    std::visit(
        overloaded{
            [&](PointLight l) { l.on(); },
            [&](DirectionalLight l) { l.on(); },
            [&](SpotLight l) { l.on(); },
        },
        _l);
}
