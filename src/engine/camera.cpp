#include "engine/camera.hpp"

#include <cmath>
#ifdef __APPLE__
#    include <GLUT/glut.h>
#else
#    include <GL/glew.h>
#    include <GL/glut.h>
#endif


void Camera::react_key(unsigned char key, int x, int y) {
    auto v = VectorSpherical(Vector(_center, _pl));
    auto t = VectorSpherical(0, 0, 0);

    switch (key) {
        case '+':
            _pl = _center + v.add_radius(-0.1);
            break;
        case '-':
            _pl = _center + v.add_radius(0.1);
            break;
        case 'k':
            if (v.inclination() + 0.1 < M_PI)
                _pl = _center + v.add_inclination(0.1);
            break;
        case 'j':
            if (v.inclination() - 0.1 > 0)
                _pl = _center + v.add_inclination(-0.1);
            break;
        case 'l':
            _pl = _center + v.add_azimuth(-0.1);
            break;
        case 'h':
            _pl = _center + v.add_azimuth(0.1);
            break;
        case 'w':
            t = VectorSpherical(0.1, M_PI / 2, v.azimuth() + M_PI);
            _pl = _pl + t;
            _center = _center + t;
            break;
        case 's':
            t = VectorSpherical(0.1, M_PI / 2, v.azimuth());
            _center = _center + t;
            _pl = _center + v;
            break;
        case 'a':
            t = VectorSpherical(0.1, M_PI / 2, v.azimuth() - M_PI / 2);
            _center = _center + t;
            _pl = _center + v;
            break;
        case 'd':
            t = VectorSpherical(0.1, M_PI / 2, v.azimuth() + M_PI / 2);
            _center = _center + t;
            _pl = _center + v;
            break;
        case 'r': // reset camera
            _center = Point(0, 0, 0);
            _pl = _center + v;
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
        _pl.x(),
        _pl.y(),
        _pl.z(),
        _center.x(),
        _center.y(),
        _center.z(),
        0.0f,
        1.0f,
        0.0f);
}
