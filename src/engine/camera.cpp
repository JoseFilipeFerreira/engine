#include "engine/camera.hpp"

#include <cmath>
#include <iostream>
#ifdef __APPLE__
#    include <GLUT/glut.h>
#else
#    include <GL/glew.h>
#    include <GL/glut.h>
#endif

void Camera::react_key_orbit(unsigned char key, int x, int y) {
    auto v = VectorSpherical(Vector(_pl, _pc));
    auto t = VectorSpherical(0, 0, 0);

    switch (key) {
        case '+':
            _pc = _pl + v.add_radius(-0.1);
            break;
        case '-':
            _pc = _pl + v.add_radius(0.1);
            break;
        case 'j':
            if (v.inclination() + 0.1 < M_PI)
                _pc = _pl + v.add_inclination(0.1);
            break;
        case 'k':
            if (v.inclination() - 0.1 > 0) _pc = _pl + v.add_inclination(-0.1);
            break;
        case 'h':
            _pc = _pl + v.add_azimuth(-0.1);
            break;
        case 'l':
            _pc = _pl + v.add_azimuth(0.1);
            break;
        case 'w':
            t = VectorSpherical(0.1, M_PI / 2, v.azimuth() + M_PI);
            _pc = _pc + t;
            _pl = _pl + t;
            break;
        case 's':
            t = VectorSpherical(0.1, M_PI / 2, v.azimuth());
            _pl = _pl + t;
            _pc = _pl + v;
            break;
        case 'a':
            t = VectorSpherical(0.1, M_PI / 2, v.azimuth() - M_PI / 2);
            _pl = _pl + t;
            _pc = _pl + v;
            break;
        case 'd':
            t = VectorSpherical(0.1, M_PI / 2, v.azimuth() + M_PI / 2);
            _pl = _pl + t;
            _pc = _pl + v;
            break;
        case 'r': // reset camera
            _pl = Point(0, 0, 0);
            _pc = _pl + v;
            break;
    }
}

void Camera::react_key_fpv(unsigned char key, int x, int y) {
    auto v = VectorSpherical(_pc, _pl);
    auto vc = Vector(_pc, _pl);
    auto vn = v.normalize();
    switch (key) {
        case 'j':
            if (vn.inclination() + 0.05 < M_PI)
                _pl = _pc + vn.add_inclination(0.05) * v.radius();
            break;
        case 'k':
            if (vn.inclination() - 0.05 > 0)
                _pl = _pc + vn.add_inclination(-0.05) * v.radius();
            break;
        case 'h':
            _pl = _pc + vn.add_azimuth(0.05) * v.radius();
            break;
        case 'l':
            _pl = _pc + vn.add_azimuth(-0.05) * v.radius();
            break;
        case 'w':
            _pl = _pl + vn * 0.05f;
            _pc = _pc + vn * 0.05f;
            break;
        case 's':
            _pl = _pl + vn * -0.05f;
            _pc = _pc + vn * -0.05f;
            break;
        case 'a':
            _pc = _pc + vc.cross(Vector(0, -1, 0)).normalize() * 0.05;
            _pl = _pc + vc;
            break;
        case 'd':
            _pc = _pc + vc.cross(Vector(0, 1, 0)).normalize() * 0.05;;
            _pl = _pc + vc;
            break;
    }
}

void Camera::react_key(unsigned char key, int x, int y) {
    switch (_camera_mode) {
        case CameraMode::Orbit:
            react_key_orbit(key, x, y);
            break;
        case CameraMode::Fpv:
            react_key_fpv(key, x, y);
            break;
    }

    switch (key) {
        case '1':
            if (_camera_mode != CameraMode::Orbit) {
                _camera_mode = CameraMode::Orbit;
            }
            break;
        case '2':
            if (_camera_mode != CameraMode::Fpv) {
                _camera_mode = CameraMode::Fpv;
            }
            break;
    }
}

void draw_point(Point p) {
    glPushMatrix();
    glColor3f(0, 1, 0);
    glTranslatef(p.x(), p.y(), p.z());
    glutSolidSphere(0.05, 10, 10);
    glPopMatrix();
}

void Camera::place_camera() const {
    glLoadIdentity();
    gluLookAt(
        _pc.x(), _pc.y(), _pc.z(), _pl.x(), _pl.y(), _pl.z(), 0.0f, 1.0f, 0.0f);
}

auto operator<<(std::ostream& oss, Camera const& c) -> std::ostream& {
    oss << "| CAMERA MODE: ";
    switch (c._camera_mode) {
        case CameraMode::Orbit:
            oss << "Orbit";
            break;
        case CameraMode::Fpv:
            oss << "Fpv";
            break;
    }
    oss << " ";
    return oss;
}
