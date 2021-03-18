#ifndef SHADER_HPP
#define SHADER_HPP

#include <iostream>

#include "GL/glew.h"
#include "tool.hpp"

namespace tinyrenderer3d {

enum class ShaderType {
    VERTEX_SHADER,
    FRAGMENT_SHADER
};

class Shader final {
 public:
     Shader(ShaderType type, string shader_code);
     GLuint GetShaderBuffer() const {
         return shader_;
     }
     ~Shader();

 private:
     GLuint shader_;
};

}; // NAMESPACE tinyrender3d

#endif
