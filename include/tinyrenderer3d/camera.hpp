#ifndef TINYRENDERER3D_CAMERA_HPP
#define TINYRENDERER3D_CAMERA_HPP

#include "vmath.hpp"
#include "log.hpp"

namespace tinyrenderer3d {

class Camera final {
 public:
    Camera();
    ~Camera() = default;

    Mat4<float> GetMatrix() const;

    void MoveTo(const Vec3<float>& position);
    void MoveByWorld(const Vec3<float>& offset);
    void MoveByMyself(const Vec3<float>& offset);
    void SetRotation(float degree);
    void RotateBy(float degree);

    Vec3<float> GetPosition() const { return position_; }
    float GetRotation() const { return angle_; }

 private:
    Vec3<float> front_;

    Vec3<float> position_;
    float angle_;

    Mat4<float> calcMatrixByEular() const;
    void calcFront();
};

}

#endif
