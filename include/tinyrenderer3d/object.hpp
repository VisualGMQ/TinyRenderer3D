#ifndef TINYRENDERER3D_OBJECT_HPP
#define TINYRENDERER3D_OBJECT_HPP

#define GLEW_STATIC
#include "GL/glew.h"

#include "tinyrenderer3d/drawable.hpp"
#include "tinyrenderer3d/vmath.hpp"

namespace tinyrenderer3d {

class Cube: public Drawable {
 public:
    Vec3<float> center = {0, 0, 0};
    float l = 1;
    Rotation3d<float> rotation = {0, 0, 0};

    void Draw(Program* program) override;
};

class Cuboid: public Drawable {
 public:
    Vec3<float> center = {0, 0, 0};
    Size3d<float> size = {1, 1, 1};
    Rotation3d<float> rotation = {0, 0, 0};

    void Draw(Program* program) override;
};

class Plane: public Drawable {
 public:
    Vec3<float> center = {0, 0, 0};
    Size2d<float> size = {1, 1};
    Rotation3d<float> rotation = {0, 0, 0};

    void Draw(Program* program) override;
};

}

#endif
