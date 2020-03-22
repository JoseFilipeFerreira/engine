#ifndef CAMERA_H
#define CAMERA_H

#include "utils/point.hpp"

class Camera {
  private:
    PointSpherical _pl;
    Point _center;

  public:
    Camera();
    void react_key(unsigned char, int, int);
    void place_camera() const;
};

#endif // CAMERA_H
