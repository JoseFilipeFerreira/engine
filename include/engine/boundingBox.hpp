#ifndef BOUNDINBOX_H
#define BOUNDINBOX_H

#include "utils/point.hpp"
#include "utils/types.hpp"

#include <iostream>
#include <vector>
#ifdef __APPLE__
#    include <GLUT/glut.h>
#else
#    include <GL/glew.h>
#    include <GL/glut.h>
#endif

class BoundingBox {
  private:
    float _minX, _minY, _minZ;
    float _maxX, _maxY, _maxZ;

  public:
    BoundingBox(): _minX(0), _minY(0), _minZ(0), _maxX(0), _maxY(0), _maxZ(0) {}
    BoundingBox(std::vector<float> const& vec) {
        _minX = _maxX = vec[0];
        _minY = _maxY = vec[1];
        _minZ = _maxZ = vec[2];
        for (i64 i = 3; i < vec.size(); i += 3) {
            _minX = std::min(_minX, vec[i]);
            _maxX = std::max(_maxX, vec[i]);
            _minY = std::min(_minY, vec[i + 1]);
            _maxY = std::max(_maxY, vec[i + 1]);
            _minZ = std::min(_minZ, vec[i + 2]);
            _maxZ = std::max(_maxZ, vec[i + 2]);
        }
    }

    auto is_visible() const -> bool { return true; }

    void draw() const {
        float cc[4];
        glGetFloatv(GL_CURRENT_COLOR, cc);
        glColor3f(1, 1, 1);

        glBegin(GL_LINES);
        glVertex3f(_minX, _minY, _minZ);
        glVertex3f(_minX, _minY, _maxZ);

        glVertex3f(_minX, _minY, _minZ);
        glVertex3f(_maxX, _minY, _minZ);

        glVertex3f(_maxX, _minY, _minZ);
        glVertex3f(_maxX, _minY, _maxZ);

        glVertex3f(_minX, _minY, _maxZ);
        glVertex3f(_maxX, _minY, _maxZ);

        glVertex3f(_minX, _maxY, _minZ);
        glVertex3f(_minX, _maxY, _maxZ);

        glVertex3f(_minX, _maxY, _minZ);
        glVertex3f(_maxX, _maxY, _minZ);

        glVertex3f(_maxX, _maxY, _minZ);
        glVertex3f(_maxX, _maxY, _maxZ);

        glVertex3f(_minX, _maxY, _maxZ);
        glVertex3f(_maxX, _maxY, _maxZ);

        glVertex3f(_minX, _minY, _minZ);
        glVertex3f(_minX, _maxY, _minZ);

        glVertex3f(_maxX, _minY, _minZ);
        glVertex3f(_maxX, _maxY, _minZ);

        glVertex3f(_minX, _minY, _maxZ);
        glVertex3f(_minX, _maxY, _maxZ);

        glVertex3f(_maxX, _minY, _maxZ);
        glVertex3f(_maxX, _maxY, _maxZ);
        glEnd();

        glColor4fv(cc);
    }
};

#endif // BOUNDINBOX_H
