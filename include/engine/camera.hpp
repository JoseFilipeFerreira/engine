#ifndef CAMERA_H
#define CAMERA_H

#include "utils/point.hpp"

class Camera {
  private:
    PointSpherical _pl;
    Point _center;
    bool _has_axis;
    bool _display_center;

  public:
    Camera()
        : _pl(PointSpherical(Point(10, 10, 10))),
          _center(Point(0, 0, 0)),
          _has_axis(false),
          _display_center(false){};
    void react_key(unsigned char, int, int);
    void place_camera() const;
};

#endif // CAMERA_H
