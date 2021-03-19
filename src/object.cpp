#include "tinyrenderer3d/object.hpp"

namespace tinyrenderer3d {

void Plane::Draw(Program* program) {
    const static float data[] = {
        -0.5, 0,  0.5, 0, 0, 0, 1, 0,
         0.5, 0,  0.5, 1, 0, 0, 1, 0,
         0.5, 0, -0.5, 1, 1, 0, 1, 0,

        -0.5, 0,  0.5, 0, 0, 0, 1, 0,
         0.5, 0, -0.5, 1, 1, 0, 1, 0,
        -0.5, 0, -0.5, 0, 1, 0, 1, 0
    };

    Mat4<float> model = Mat4<float>(1.0f);
    model = glm::translate(model, center);
    model = model*GetRotateMatByQuat(rotation);
    model = glm::scale(model, Vec3<float>(size.w, 0, size.h));

    GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo_));
    GLCall(glBindVertexArray(vao_));

    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW));

    if (HasTexture()){
        GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, false, 8*sizeof(float), 0));
        GLCall(glEnableVertexAttribArray(0));
        GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, false, 8*sizeof(float), (void*)(3*sizeof(float))));
        GLCall(glEnableVertexAttribArray(1));
        GLCall(glVertexAttribPointer(2, 3, GL_FLOAT, false, 8*sizeof(float), (void*)(5*sizeof(float))));
        GLCall(glEnableVertexAttribArray(2));
    } else {
        GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, false, 8*sizeof(float), 0));
        GLCall(glEnableVertexAttribArray(0));
        GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, false, 8*sizeof(float), (void*)(5*sizeof(float))));
        GLCall(glEnableVertexAttribArray(1));
    }
    program->UniformMat4f("model", model);
    material.Use(program);

    GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));

    material.DontUse();

    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GLCall(glBindVertexArray(0));
}

void Cube::Draw(Program* program){
    static const float data[] = {
        // position           texcoord     normal
        // Back face
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0, 0, -1,    // Bottom-left
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0, 0, -1,    // top-right
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0, 0, -1,    // bottom-right         
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0, 0, -1,    // top-right
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0, 0, -1,    // bottom-left
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0, 0, -1,    // top-left
        // Front face
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0, 0, 1,     // bottom-left
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0, 0, 1,     // bottom-right
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0, 0, 1,     // top-right
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0, 0, 1,     // top-right
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0, 0, 1,     // top-left
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0, 0, 1,     // bottom-left
        // Left face
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  -1, 0, 0,    // top-right
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  -1, 0, 0,    // top-left
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  -1, 0, 0,    // bottom-left
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  -1, 0, 0,    // bottom-left
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  -1, 0, 0,    // bottom-right
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  -1, 0, 0,    // top-right
        // Right face
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1, 0, 0,     // top-left
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1, 0, 0,     // bottom-right
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1, 0, 0,     // top-right         
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1, 0, 0,     // bottom-right
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1, 0, 0,     // top-left
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1, 0, 0,     // bottom-left     
        // Bottom face
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0, -1, 0,    // top-right
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  0, -1, 0,    // top-left
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0, -1, 0,    // bottom-left
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0, -1, 0,    // bottom-left
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0, -1, 0,    // bottom-right
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0, -1, 0,    // top-right
        // Top face
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0, 1, 0,     // top-left
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0, 1, 0,     // bottom-right
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0, 1, 0,     // top-right     
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0, 1, 0,     // bottom-right
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0, 1, 0,     // top-left
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  0, 1, 0      // bottom-left
    };

    Mat4<float> model = Mat4<float>(1.0f);
    model = glm::translate(model, center);
    model = model*GetRotateMatByQuat(rotation);
    model = glm::scale(model, Vec3<float>(l));

    GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo_));
    GLCall(glBindVertexArray(vao_));

    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW));

    if (HasTexture()) {
        GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, false, 8*sizeof(float), 0));
        GLCall(glEnableVertexAttribArray(0));
        GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, false, 8*sizeof(float), (void*)(3*sizeof(float))));
        GLCall(glEnableVertexAttribArray(1));
        GLCall(glVertexAttribPointer(2, 3, GL_FLOAT, false, 8*sizeof(float), (void*)(5*sizeof(float))));
        GLCall(glEnableVertexAttribArray(2));
    } else {
        GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, false, 8*sizeof(float), 0));
        GLCall(glEnableVertexAttribArray(0));
        GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, false, 8*sizeof(float), (void*)(5*sizeof(float))));
        GLCall(glEnableVertexAttribArray(1));
    }

    program->UniformMat4f("model", model);
    material.Use(program);

    GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));

    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GLCall(glBindVertexArray(0));

}

void Cuboid::Draw(Program* program) {}

}
