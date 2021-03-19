#include "tinyrenderer3d/drawable.hpp"

namespace tinyrenderer3d {


Drawable::Drawable() {
    GLCall(glGenBuffers(1, &vbo_));
    GLCall(glGenBuffers(1, &ebo_));
    GLCall(glGenVertexArrays(1, &vao_));
}

Drawable::~Drawable() {
    GLCall(glDeleteBuffers(1, &vbo_));
    GLCall(glDeleteBuffers(1, &ebo_));
    GLCall(glDeleteVertexArrays(1, &vao_));
}

}
