#include "tinyrenderer3d/material.hpp"

namespace tinyrenderer3d {

void Material::Use(Program* program) {
    if (HasTexture()) {
        textures.at(0)->UseAsTexture();
        GLCall(glActiveTexture(GL_TEXTURE0));
        program->Uniform1i("material.diffuse", 0);
    } else {
        program->UniformVec3f("material.diffuse", ConvertColor4To3(ConvertColor255To01<float>(diffuse)));
    }
    program->UniformVec3f("material.ambient", ConvertColor4To3(ConvertColor255To01<float>(ambient)));
    program->UniformVec3f("material.specular", ConvertColor4To3<float>(ConvertColor255To01<float>(specular)));
    program->Uniform1f("material.shininess", shininess);
}

void Material::DontUse() {
    if (HasTexture()) {
        GLCall(glBindTexture(GL_TEXTURE_2D, 0));
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    }
}

}