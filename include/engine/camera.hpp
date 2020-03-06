#ifndef CAMERA_H
#define CAMERA_H

#include "utils/point.hpp"
#include <vector>

class Camera {
private:
    PointSpherical _pl;
    Point _center;

public:
    Camera();
    void react_key(unsigned char, int, int);
    void place_camera();

};

#endif // CAMERA_H
