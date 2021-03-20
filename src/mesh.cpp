#include "tinyrenderer3d/mesh.hpp"

namespace tinyrenderer3d {

Mesh::Mesh() {
    GLCall(glGenBuffers(1, &vbo_));
    GLCall(glGenBuffers(1, &ebo_));
    GLCall(glGenVertexArrays(1, &vao_));
}

Mesh::~Mesh() {
    GLCall(glDeleteBuffers(1, &vbo_));
    GLCall(glDeleteBuffers(1, &ebo_));
    GLCall(glDeleteVertexArrays(1, &vao_));
}

void Mesh::Draw(Program* program) {
    program->UniformMat4f("model", calcModel());

    bindBuffers();

    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertices.size(), vertices.data(), GL_STATIC_DRAW));
    if (!indices.empty()) {
        GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*indices.size(), indices.data(), GL_STATIC_DRAW));
    }

    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position)));
    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texcoord)));
    GLCall(glEnableVertexAttribArray(1));
    GLCall(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal)));
    GLCall(glEnableVertexAttribArray(2));

    material.Use(program);

    if (!indices.empty()) {
        GLCall(glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0));
    } else {
        GLCall(glDrawArrays(GL_TRIANGLES, 0, vertices.size()));
    }
    
    material.DontUse();
    unbindBuffers();
}

void Mesh::bindBuffers() {
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo_));
    GLCall(glBindVertexArray(vao_));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_));
}

void Mesh::unbindBuffers() {
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GLCall(glBindVertexArray(0));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

Mat4<float> Mesh::calcModel() {
    Mat4<float> model = Mat4<float>(1.0f);
    model = glm::translate(model, center);
    model = glm::rotate(model, glm::radians(rotation.x), Vec3<float>(1, 0, 0));
    model = glm::rotate(model, glm::radians(rotation.y), Vec3<float>(0, 1, 0));
    model = glm::rotate(model, glm::radians(rotation.z), Vec3<float>(0, 0, 1));
    model = glm::scale(model, scale);
    return model;
}

}
