#ifndef CAMERA_H
#define CAMERA_H

#include "utils/point.hpp"

class Camera {
  private:
    Point _pl;
    Point _center;

  public:
    Camera()
        : _pl(Point(10, 10, 10)), _center(Point(0, 0, 0)) {}

    template<typename T> auto with (T func) -> void {
        func(_pl, _center);
    };
    void react_key(unsigned char, int, int);
    void place_camera() const;
};

#endif // CAMERA_H
