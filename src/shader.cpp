#include "tinyrenderer3d/shader.hpp"

namespace tinyrenderer3d {

Shader::Shader(ShaderType type, string shader_code) {
    if (type == ShaderType::VERTEX_SHADER) {
        shader_ = glCreateShader(GL_VERTEX_SHADER);
    }else if (type == ShaderType::FRAGMENT_SHADER) {
        shader_ = glCreateShader(GL_FRAGMENT_SHADER);
    }
    const char* const shader_source = shader_code.c_str();
    glShaderSource(shader_, 1, &shader_source, nullptr);
    glCompileShader(shader_);
    int success;
    glGetShaderiv(shader_, GL_COMPILE_STATUS, &success);
    if (!success) {
        char buf[1024] = {};
        glGetShaderInfoLog(shader_, sizeof(buf)-1, nullptr, buf);
        if (type == ShaderType::VERTEX_SHADER)
            std::cout << "[vertex shader]: shader compile error:" << std::endl;
        else if(type == ShaderType::FRAGMENT_SHADER)
            std::cout << "[fragment shader]: shader compile error:" << std::endl;
        std::cout << buf << std::endl;
        throw std::runtime_error("shader compile failed");
    }
}

Shader::~Shader() {
    glDeleteShader(shader_);
}

};
