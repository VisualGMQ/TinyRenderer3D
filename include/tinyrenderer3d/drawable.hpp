#ifndef TINYRENDERER3D_DRAWABLE_HPP
#define TINYRENDERER3D_DRAWABLE_HPP

#define GLEW_STATIC
#include "GL/glew.h"

#include "program.hpp"
#include "material.hpp"

namespace tinyrenderer3d {

class Drawable {
 public:
    Drawable();
    virtual ~Drawable();

    bool HasTexture() const { return material.HasTexture(); }
    virtual void Draw(Program* program) = 0;

    Material material;

 protected:
    GLuint vbo_ = 0;
    GLuint ebo_ = 0;
    GLuint vao_ = 0;
};

}

#endif
