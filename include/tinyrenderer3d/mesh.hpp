#ifndef TINYRENDERER3D_MESH_HPP
#define TINYRENDERER3D_MESH_HPP

#include <vector>

#include "drawable.hpp"
#include "vmath.hpp"
#include "program.hpp"
#include "tool.hpp"
#include "drawable.hpp"

namespace tinyrenderer3d {

using std::vector;

class Mesh: public Drawable {
 public:
    Mesh();
    ~Mesh();

    void Draw(Program* program) override;

    vector<Vertex> vertices;
    vector<uint> indices;
    Material material;

    Vec3<float> center = {0, 0, 0};
    Vec3<float> rotation = {0, 0, 0};
    Vec3<float> scale = {1, 1, 1};

 private:
    GLuint vbo_ = 0;
    GLuint ebo_ = 0;
    GLuint vao_ = 0;

    void bindBuffers();
    void unbindBuffers();
    Mat4<float> calcModel();
};

}

#endif
