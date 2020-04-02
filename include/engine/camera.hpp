#ifndef CAMERA_H
#define CAMERA_H

#include "utils/point.hpp"

class Camera {
  private:
    Point _pl;
    Point _center;
    bool _has_axis;

  public:
    Camera()
        : _pl(Point(10, 10, 10)), _center(Point(0, 0, 0)), _has_axis(false) {}
    template<typename T> auto with (T func) -> void {
        func(_pl, _center, _has_axis);
    };
    void react_key(unsigned char, int, int);
    void place_camera(bool) const;
};

#endif // CAMERA_H
