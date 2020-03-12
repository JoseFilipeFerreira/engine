#include "engine/camera.hpp"

#include <GL/gl.h>
#ifdef __APPLE__
#    include <GLUT/glut.h>
#else
#    include <GL/glut.h>
#endif

Camera::Camera()
    : _pl(PointSpherical(Point(10, 10, 10))), _center(Point(0, 0, 0)) {}

void Camera::react_key(unsigned char key, int x, int y) {
    switch (key) {
        case '+':
            _pl.radius(-0.1);
            break;
        case '-':
            _pl.radius(0.1);
            break;
        case 'j':
            _pl.inclination(0.1);
            break;
        case 'k':
            _pl.inclination(-0.1);
            break;
        case 'l':
            _pl.azimuth(0.1);
            break;
        case 'h':
            _pl.azimuth(-0.1);
            break;
            /*
            case 'w':
                _center.z(0.1);
                break;
            case 's':
                _center.z(-0.1);
                break;
            case 'a':
                _center.x(0.1);
                break;
            case 'd':
                _center.x(-0.1);
                break;
                */
    }
}

void Camera::place_camera() const {
    Point c = Point(_pl);
    glLoadIdentity();
    gluLookAt(
        c.x(),
        c.y(),
        c.z(),
        _center.x(),
        _center.y(),
        _center.z(),
        0.0f,
        1.0f,
        0.0f);
}
