#ifndef TINYRENDERER3D_CAMERA_HPP
#define TINYRENDERER3D_CAMERA_HPP

#include "vmath.hpp"
#include "log.hpp"

namespace tinyrenderer3d {

// FIXME something was wrong. the `MoveByMyself()` have some error
class Camera final {
 public:
    Camera();
    ~Camera() = default;

    Mat4<float> GetMatrix() const;

    void MoveTo(const Vec3<float>& position);
    void MoveByWorld(const Vec3<float>& offset);
    void MoveByMyself(const Vec3<float>& offset);
    void SetTarget(const Vec3<float>& target);
    void SetEularAngles(float pitch, float yaw, float roll = 0);
    void RotateBy(float pitch_offset, float yaw_offset, float roll_offset);

    Vec3<float> GetPosition() const { return position_; }
    EularAngle<float> GetEularAngle() const { return eular_angle_; }

 private:
    Vec3<float> direction_;
    Vec3<float> up_;
    Vec3<float> right_;

    Vec3<float> position_;
    Vec3<float> up_vec_;
    EularAngle<float> eular_angle_;

    Mat4<float> calcMatrixByEular() const;
    Mat4<float> getMyselfCoord() const;
    void calcCoordVecs();
    void calcDirection();
    void calcUp();
    void calcRight(const Vec3<float>& direction, const Vec3<float>& up);
};

}

#endif
