#ifndef TINYRENDERER3D_DRAWABLE_HPP
#define TINYRENDERER3D_DRAWABLE_HPP

#define GLEW_STATIC
#include "GL/glew.h"

#include "program.hpp"
#include "material.hpp"

namespace tinyrenderer3d {

class Drawable {
 public:
    Drawable() = default;
    virtual ~Drawable() = default;

    virtual void Draw(Program*) = 0;
};

}

#endif
