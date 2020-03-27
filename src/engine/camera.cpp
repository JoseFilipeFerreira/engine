#include "engine/camera.hpp"

#include <GL/gl.h>
#include <cmath>
#ifdef __APPLE__
#    include <GLUT/glut.h>
#else
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
        case 'j':
            if (v.inclination() + 0.1 < M_PI)
                _pl = _center + v.add_inclination(0.1);
            break;
        case 'k':
            if (v.inclination() - 0.1 > 0)
                _pl = _center + v.add_inclination(-0.1);
            break;
        case 'l':
            _pl = _center + v.add_azimuth(0.1);
            break;
        case 'h':
            _pl = _center + v.add_azimuth(-0.1);
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
        case 'x': // toggle axis
            _has_axis = !_has_axis;
            break;
        case 'g': // toggle debug mode
            if (_has_axis && _display_center) {
                _has_axis = false;
                _display_center = false;
            } else {
                _has_axis = true;
                _display_center = true;
            }
            break;
    }
}

void draw_axis() {
    // axis
    glBegin(GL_LINES);
    // X axis in red
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-100.0f, 0.0f, 0.0f);
    glVertex3f(100.0f, 0.0f, 0.0f);
    // Y Axis in Green
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, -100.0f, 0.0f);
    glVertex3f(0.0f, 100.0f, 0.0f);
    // Z Axis in Blue
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, -100.0f);
    glVertex3f(0.0f, 0.0f, 100.0f);
    glEnd();
}

void draw_point(Point p) {
    glPushMatrix();
    glColor3f(0, 1, 0);
    glTranslatef(p.x(), p.y(), p.z());
    glutSolidSphere(0.05, 10, 10);
    glPopMatrix();
}

void Camera::place_camera() const {
    auto c = Point(_pl);
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

    if (_has_axis) draw_axis();
    if (_display_center) draw_point(_center);
}
