#ifndef TINYRENDERER3D_PROGRAM_HPP
#define TINYRENDERER3D_PROGRAM_HPP

#include <string>

#include "shader.hpp"
#include "vmath.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "log.hpp"

namespace tinyrenderer3d {

using std::string;

constexpr const char* TextureProgramName = "texture_program";
constexpr const char* ShadowProgramName = "shadow_program";
constexpr const char* SkyboxProgramName = "skybox_program";

class Program {
 public:
    Program(string name, Shader& vertex_shader, Shader& frag_shader);
    ~Program();

    const string& GetName() const { return name_; }
    void UniformMat4f(const string& name, const Mat4<float>& value);
    void UniformVec2f(const string& name, const Vec2<float>& value);
    void UniformVec3f(const string& name, const Vec3<float>& value);
    void UniformVec4f(const string& name, const Vec4<float>& value);
    void UniformVec4i(const string& name, const Vec4<int>& value);
    void Uniform1i(const string& name, int value);
    void Uniform1ui(const string& name, uint value);
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

    string name_;
};

Program* CreateProgram(string name, string vertex_filename, string frag_filename);
void DestroyProgram(Program* program);

}; // NAMESPACE tinyrender3d

#endif
