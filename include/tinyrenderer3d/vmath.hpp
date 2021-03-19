#ifndef TINYRENDERER3D_VMATH_HPP
#define TINYRENDERER3D_VMATH_HPP

#include <cstdint>
#include <type_traits>

#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/trigonometric.hpp"
#include "tinyrenderer3d/types.hpp"

namespace tinyrenderer3d {

template <int length, typename T>
using Vec = glm::vec<length, T>;

template <typename T>
using Vec2 = Vec<2, T>;

template <typename T>
using Vec3 = Vec<3, T>;

template <typename T>
using Vec4 = Vec<4, T>;

template <typename T>
using Mat3 = glm::mat<3, 3, T>;

template <typename T>
using Mat4 = glm::mat<4, 4, T>;

template <typename T>
using Point2d = Vec2<T>;

template <typename T>
using Point3d = Vec3<T>;

template <typename T>
using Color4 = Vec4<T>;

using Color = Color4<u8>;

template <typename T>
using Color3 = Vec3<T>;

template <typename T>
Vec4<T> ConvertColor255To01(const Color& color) {
    return {color.r/255.0, color.g/255.0, color.b/255.0, color.a/255.0};
}

template <typename T>
Color3<T> ConvertColor4To3(const Color4<T>& color) {
    return {color.r, color.g, color.b};
}

struct ColorfulPoint {
    Point3d<float> point;
    Color color;
};

template <typename T>
using Rotation2d = T;

template <typename T>
using Rotation3d = Vec3<T>;

template <typename T>
using Quaternion = glm::qua<T>;

template <typename T>
Mat4<T> GetRotateMatByQuat(const Rotation3d<T>& rotation) {
    auto m = Mat4<T>(1.0);
    Quaternion<T> quat = Quaternion<T>(glm::radians(rotation));
    return glm::mat4_cast(quat)*m;
}

template <typename T>
Mat4<T> GetRotateMatByEular(const Rotation3d<T>& rotation) {
    auto m = Mat4<T>(1.0);
    m = glm::rotate(m, glm::radians(rotation.x), Vec3<T>(1, 0, 0));
    m = glm::rotate(m, glm::radians(rotation.y), Vec3<T>(0, 1, 0));
    m = glm::rotate(m, glm::radians(rotation.z), Vec3<T>(0, 0, 1));
    return m;
}

template <typename T>
struct Size2d {
    T w;
    T h;
};

template <typename T>
struct Size3d {
    T w;
    T h;
    T l;
};

template <typename T>
struct Rect {
    Point2d<T> position = {0, 0};
    Size2d<T> size = {1, 1};
    Rotation2d<T> rotation = {0};
};

template <typename T>
struct EularAngle {
    T pitch = 0;
    T yaw = 0;
    T roll = 0;
};


}; // NAMESPACE tinyrender3d

#endif
