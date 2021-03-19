#include "tinyrenderer3d/camera.hpp"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"

namespace tinyrenderer3d {

Camera::Camera():position_({0, 0, 5}), angle_(0) {
    calcFront();
}

Mat4<float> Camera::GetMatrix() const {
    return calcMatrixByEular();
}

void Camera::MoveByWorld(const Vec3<float>& offset) {
    position_ += offset;
}

void Camera::MoveByMyself(const Vec3<float>& offset) {
    Vec3<float> up(0, 1, 0);
    Vec3<float> right = glm::cross(up, front_);

    position_ -= offset.x*right;
    position_ -= offset.y*up;
    position_ -= offset.z*front_;
}

void Camera::MoveTo(const Vec3<float>& position) {
    position_ = position;
}

void Camera::SetRotation(float degree) {
    angle_ = degree;
    calcFront();
}

void Camera::RotateBy(float degree) {
    angle_ += degree;
    calcFront();
}

void Camera::calcFront() {
    front_.x = glm::sin(glm::radians(180+angle_));
    front_.y = 0;
    front_.z = glm::cos(glm::radians(180+angle_));
}

Mat4<float> Camera::calcMatrixByEular() const {
    Vec3<float> up(0, 1, 0);
    return glm::lookAt(position_, position_ + front_, up);
}

}
