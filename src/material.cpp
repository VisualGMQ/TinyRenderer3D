#include "tinyrenderer3d/material.hpp"

namespace tinyrenderer3d {

void Material::Use(Program* program) {
    if (normal_texture) {
        GLCall(glActiveTexture(GL_TEXTURE0));
        normal_texture->UseAsTexture();
        GLCall(program->Uniform1i("material.normal_texture", 0));
        GLCall(program->Uniform1i("material.normal_texture_num", 1));
    } else {
        GLCall(program->Uniform1i("material.normal_texture_num", 0));
    }
    if (diffuse_texture) {
        GLCall(glActiveTexture(GL_TEXTURE1));
        diffuse_texture->UseAsTexture();
        GLCall(program->Uniform1i("material.diffuse_texture", 1));
        GLCall(program->Uniform1i("material.diffuse_texture_num", 1));
    } else {
        GLCall(program->Uniform1i("material.diffuse_texture_num", 0));
    }
    if (specular_texture) {
        GLCall(glActiveTexture(GL_TEXTURE2));
        specular_texture->UseAsTexture();
        GLCall(program->Uniform1i("material.specular_texture", 2));
        GLCall(program->Uniform1i("material.specular_texture_num", 1));
    } else {
        GLCall(program->Uniform1i("material.specular_texture_num", 0));
    }

    program->UniformVec3f("material.diffuse", diffuse);
    program->UniformVec3f("material.ambient", ambient);
    program->UniformVec3f("material.specular", specular);
    program->Uniform1f("material.shininess", shininess);
}

void Material::DontUse() {
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

}
