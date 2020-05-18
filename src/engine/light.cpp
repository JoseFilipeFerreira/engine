#include "engine/light.hpp"

#include "utils/types.hpp"

#include <iostream>

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

void PointLight::on(GLenum light) const {
    glEnable(light);

    float pos[4] = {_position.x(), _position.y(), _position.z(), 1.0f};

    auto const ambient = _colour * 0.2f;

    GLfloat amb[4] = { ambient.r(), ambient.g(), ambient.b(), 1.0f };
    GLfloat diff[4] = { _colour.r(), _colour.g(), _colour.b(), 1.0 };
    glLightfv(light, GL_POSITION, pos);
    glLightfv(light, GL_AMBIENT, amb);
    glLightfv(light, GL_DIFFUSE, diff);

    GLfloat atten = 0.0f;
    glLightfv(light, GL_LINEAR_ATTENUATION, &atten);
}

void DirectionalLight::on(GLenum light) const {
    glEnable(light);

    auto const ambient = _colour * 0.2f;

    GLfloat amb[4] = { ambient.r(), ambient.g(), ambient.b(), 1.0f };
    GLfloat diff[4] = { _colour.r(), _colour.g(), _colour.b(), 1.0 };
    GLfloat dir[4] = {_direction.x(), _direction.y(), _direction.z(), 0.0f};

    glLightfv(light, GL_POSITION, dir);
    glLightfv(light, GL_AMBIENT, amb);
    glLightfv(light, GL_DIFFUSE, diff);

}

void SpotLight::on(GLenum light) const {
    glEnable(light);

    GLfloat diff[4] = { _colour.r(), _colour.g(), _colour.b(), 1.0f };

    GLfloat pos[4] = {_position.x(), _position.y(), _position.z(), 1.0f};
    GLfloat dir[3] = {_direction.x(), _direction.y(), _direction.z()};

    glLightfv(light, GL_POSITION, pos);
    glLightfv(light, GL_DIFFUSE, diff);
    glLightfv(light, GL_SPOT_DIRECTION, dir);
    glLightf(light, GL_SPOT_CUTOFF, 45.0);
    glLightf(light, GL_SPOT_EXPONENT, 0.0);
}

void Light::on() const {
    static u64 pos = 1;
    auto light = gl_light(pos);
    pos = (pos + 1) % 8;
    std::cout << pos << ' ' << light<<  '\n';
    std::visit(
        overloaded{
            [&](PointLight l) { l.on(light); },
            [&](DirectionalLight l) { l.on(light); },
            [&](SpotLight l) { l.on(light); },
        },
        _l);
}
