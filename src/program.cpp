#include "tinyrenderer3d/program.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace tinyrenderer3d {

Program::Program(Shader& vertex_shader, Shader& frag_shader) {
    program_ = glCreateProgram(); 
    GLCall(glAttachShader(program_, vertex_shader.GetShaderBuffer()));
    GLCall(glAttachShader(program_, frag_shader.GetShaderBuffer()));
    GLCall(glLinkProgram(program_));
    int success;
    GLCall(glGetProgramiv(program_, GL_LINK_STATUS, &success));
    if (!success) {
        char buf[1024] = {0};
        GLCall(glGetProgramInfoLog(program_, sizeof(buf)-1, nullptr, buf));
        std::cout << "program link error:" << std::endl;
        std::cout << buf << std::endl;
        throw std::runtime_error("program linkded failed");
    }
}

void Program::UniformMat4f(const string& name, const Mat4<float>& value) {
    GLuint loc = findUniformLocation(name);
    GLCall(glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value)));
}

void Program::UniformVec2f(const string& name, const Vec2<float>& value) {
    GLuint loc = findUniformLocation(name);
    GLCall(glUniform2fv(loc, 1, glm::value_ptr(value)));
}

void Program::UniformVec3f(const string& name, const Vec3<float>& value) {
    GLuint loc = findUniformLocation(name);
    GLCall(glUniform3fv(loc, 1, glm::value_ptr(value)));
}

void Program::UniformVec4f(const string& name, const glm::vec4& value) {
    GLuint loc = findUniformLocation(name);
    GLCall(glUniform4f(loc, value.x, value.y, value.z, value.w));
}

void Program::UniformVec4i(const string& name, const Vec4<int>& value) {
    GLuint loc = findUniformLocation(name);
    GLCall(glUniform4i(loc, value.x, value.y, value.z, value.w));
}

void Program::Uniform1i(const string& name, int value) {
    GLuint loc = findUniformLocation(name);
    GLCall(glUniform1i(loc, value));
}

void Program::Uniform1ui(const string& name, uint value) {
    GLuint loc = findUniformLocation(name);
    GLCall(glUniform1ui(loc, value));
}

void Program::Uniform1f(const string& name, float value) {
    GLuint loc = findUniformLocation(name);
    GLCall(glUniform1f(loc, value));
}

void Program::Uniform3f(const string& name, float value1, float value2, float value3) {
    GLuint loc = findUniformLocation(name);
    GLCall(glUniform3f(loc, value1, value2, value3));
}

void Program::Uniform4f(const string& name, float value1, float value2, float value3, float value4) {
    GLuint loc = findUniformLocation(name);
    GLCall(glUniform4f(loc, value1, value2, value3, value4));
}

GLint Program::findUniformLocation(const string& name) {
    GLint loc = glGetUniformLocation(program_, name.c_str());
    if (loc == -1) {
        std::cerr << name + " uniform variable not found" << std::endl;
    }
    return loc;
}


Program::~Program() {
    GLCall(glDeleteProgram(program_));
}

Program* CreateProgram(string vertex_filename, string frag_filename) {
    Shader vertex_shader(ShaderType::VERTEX_SHADER, ReadShaderFile(vertex_filename)),
           frag_shader(ShaderType::FRAGMENT_SHADER, ReadShaderFile(frag_filename));
    return new Program(vertex_shader, frag_shader);
}

void DestroyProgram(Program* program) {
    delete program;
}

};
