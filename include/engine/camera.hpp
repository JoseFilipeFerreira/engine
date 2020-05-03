#ifndef CAMERA_H
#define CAMERA_H

#include "utils/point.hpp"

enum class CameraMode { Orbit, Fpv};

class Camera {
  private:
    Point _pc;
    Point _pl;
    CameraMode _camera_mode;

  public:
    Camera()
        : _pc(10, 10, 10), _pl(0, 0, 0), _camera_mode(CameraMode::Orbit) {}

    template<typename T> auto with (T func) -> void {
        func(_pc, _pl);
    };
    void react_key(unsigned char, int, int);
    void react_key_orbit(unsigned char, int, int);
    void react_key_fpv(unsigned char, int, int);
    void place_camera() const;
    auto friend operator<<(std::ostream&, Camera const&) -> std::ostream&;
};

#endif // CAMERA_H
