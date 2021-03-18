#ifndef TINYRENDERER3D_PROGRAM_HPP
#define TINYRENDERER3D_PROGRAM_HPP

#include <string>

#include "shader.hpp"
#include "vmath.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace tinyrenderer3d {

using std::string;

class Program {
 public:
    Program(Shader& vertex_shader, Shader& frag_shader);
    ~Program();

    void UniformMat4f(const string& name, const Mat4<float>& value);
    void UniformVec2f(const string& name, const Vec2<float>& value);
    void UniformVec3f(const string& name, const Vec3<float>& value);
    void UniformVec4f(const string& name, const Vec4<float>& value);
    void UniformVec4i(const string& name, const Vec4<int>& value);
    void Uniform1i(const string& name, int value);
    void Uniform1f(const string& name, float value);
    void Uniform3f(const string& name, float value1, float value2, float value3);
    void Uniform4f(const string& name, float value1, float value2, float value3, float value4);

    GLuint GetId() const { return program_; }
    void Use() {
        glUseProgram(program_);
    }

 private:
    GLuint program_ = 0;

    GLint findUniformLocation(const string& name);
};

Program* CreateProgram(string vertex_filename, string frag_filename);
void DestroyProgram(Program* program);

}; // NAMESPACE tinyrender3d

#endif
