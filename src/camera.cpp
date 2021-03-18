#include "tinyrenderer3d/camera.hpp"
#include "glm/geometric.hpp"

namespace tinyrenderer3d {

Camera::Camera():position_({0, 0, 5}), eular_angle_({0.0f, 0.0f, 0.0f}) {
    calcCoordVecs();
}

Mat4<float> Camera::GetMatrix() const {
    return calcMatrixByEular();
}

void Camera::MoveByWorld(const Vec3<float>& offset) {
    position_ += offset;
    calcCoordVecs();
}

void Camera::MoveByMyself(const Vec3<float>& offset) {
    position_ += glm::vec3(getMyselfCoord()*glm::vec4(offset, 1.0));
}

void Camera::MoveTo(const Vec3<float>& position) {
    position_ = position;
}

void Camera::RotateBy(float pitch_offset, float yaw_offset, float roll_offset) {
    eular_angle_.pitch += pitch_offset;
    eular_angle_.yaw += yaw_offset;
    eular_angle_.roll += roll_offset;

    calcCoordVecs();
}

void Camera::SetEularAngles(float pitch, float yaw, float roll) {
    eular_angle_.pitch = pitch;
    eular_angle_.yaw = yaw;
    eular_angle_.roll = roll;

    calcMatrixByEular();
}

Mat4<float> Camera::calcMatrixByEular() const {
    auto matrix = getMyselfCoord();
    return matrix*glm::translate(matrix, -position_);
}

Mat4<float> Camera::getMyselfCoord() const {
    return Mat4<float>({
            right_.x,    right_.y,    right_.z,    0,
            up_.x,       up_.y,       up_.z,       0,
            direction_.x,direction_.y,direction_.z,0,
            0,          0,          0,          1
            });
}

void Camera::calcCoordVecs() {
    calcDirection();
    calcUp();
    calcRight(direction_, up_);
}

void Camera::calcDirection() {
    direction_.y = glm::sin(glm::radians(eular_angle_.pitch));
    direction_.x = glm::sin(glm::radians(eular_angle_.yaw))*glm::cos(glm::radians(eular_angle_.pitch));
    direction_.z = glm::cos(glm::radians(eular_angle_.yaw))*glm::cos(glm::radians(eular_angle_.pitch));
    direction_ = glm::normalize(direction_);
}

void Camera::calcUp() {
    up_.y = glm::cos(glm::radians(eular_angle_.pitch));
    up_.x = glm::sin(glm::radians(eular_angle_.yaw))*glm::sin(glm::radians(eular_angle_.pitch));
    up_.z = glm::cos(glm::radians(eular_angle_.yaw))*glm::sin(glm::radians(eular_angle_.pitch));
    up_ = glm::normalize(up_);
}

void Camera::calcRight(const Vec3<float>& direction, const Vec3<float>& up) {
    right_ = glm::cross(up, direction);
}

void Camera::SetTarget(const Vec3<float>& target) {
    // TODO not finished
    // matrix_ = glm::lookAt(position_, target, {0, 1, 0});
}

}
