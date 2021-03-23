#include "tinyrenderer3d/skybox.hpp"
#include "tinyrenderer3d/cube_map.hpp"
#include "tinyrenderer3d/program.hpp"

namespace tinyrenderer3d {

SkyBox::SkyBox(CubeMap* cubemap) {
    if (!cubemap) {
        Log("cubemap is nullptr");
    }
    map_ = cubemap;
    GLCall(glGenBuffers(1, &vbo_));
    GLCall(glGenVertexArrays(1, &vao_));
    bind();
    bufferData();
    unbind();
}

void SkyBox::bind() {
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo_));
    GLCall(glBindVertexArray(vao_));
}

void SkyBox::unbind() {
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GLCall(glBindVertexArray(0));
}

void SkyBox::bufferData() {
    float skybox_vertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f
    };
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(skybox_vertices), &skybox_vertices, GL_STATIC_DRAW));
    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0));
    GLCall(glEnableVertexAttribArray(0));
}

void SkyBox::Draw(Program* program) {
    if (program->GetName() != SkyboxProgramName) {
        Log("you must use skybox_program for drawing skybox");
        return;
    }
    bind();
    bufferData();
    GLCall(glActiveTexture(GL_TEXTURE0));
    map_->UseAsTexture();
    GLCall(glDepthMask(GL_FALSE));
    GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
    GLCall(glDepthMask(GL_TRUE));
    map_->DontUse();
    unbind();

}

SkyBox::~SkyBox() {
    unbind();
}

SkyBox* CreateSkyBox(CubeMap* cubmap) {
    return new SkyBox(cubmap);
}

void DestroySkyBox(SkyBox* box) {
    delete box;
}

}
