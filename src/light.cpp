#include "tinyrenderer3d/light.hpp"

namespace tinyrenderer3d {

void DirectionLight::Apply(Program* program) const {
    if (program) {
        program->UniformVec3f("directionLight.base.ambient", ConvertColor4To3<float>(ConvertColor255To01<float>(GetAmbient())));
        program->UniformVec3f("directionLight.base.diffuse", ConvertColor4To3<float>(ConvertColor255To01<float>(GetDiffuse())));
        program->UniformVec3f("directionLight.base.specular", ConvertColor4To3<float>(ConvertColor255To01<float>(GetSpecular())));
        program->UniformVec3f("directionLight.direction", direction_);
    }
}

void DotLight::SetParameter(float constant, float linear, float quadratic) {
    parameter_.constant = constant;
    parameter_.linear = linear;
    parameter_.quadratic = quadratic;
}

void DotLight::Apply(Program* program) const {
    program->UniformVec3f("dotLight.base.ambient", ConvertColor4To3<float>(ConvertColor255To01<float>(GetAmbient())));
    program->UniformVec3f("dotLight.base.diffuse", ConvertColor4To3<float>(ConvertColor255To01<float>(GetDiffuse())));
    program->UniformVec3f("dotLight.base.specular", ConvertColor4To3<float>(ConvertColor255To01<float>(GetSpecular())));
    program->UniformVec3f("dotLight.position", position_);
    program->Uniform1f("dotLight.attenuation.constant", parameter_.constant);
    program->Uniform1f("dotLight.attenuation.linear", parameter_.linear);
    program->Uniform1f("dotLight.attenuation.quadratic", parameter_.quadratic);
}

void SpotLight::SetParameter(float outer_cutoff, float inner_cutoff) {
    outer_cutoff_ = outer_cutoff;
    inner_cutoff_ = inner_cutoff;
}

void SpotLight::SetDirection(float x, float y, float z) {
    direction_.x = x;
    direction_.y = y;
    direction_.z = z;
}

void SpotLight::SetPosition(float x, float y, float z) {
    position_.x = x;
    position_.y = y;
    position_.z = z;
}

void SpotLight::Apply(Program* program) const {
    if (program) {
        program->UniformVec3f("spotLight.base.ambient", ConvertColor4To3<float>(ConvertColor255To01<float>(GetAmbient())));
        program->UniformVec3f("spotLight.base.diffuse", ConvertColor4To3<float>(ConvertColor255To01<float>(GetDiffuse())));
        program->UniformVec3f("spotLight.base.specular", ConvertColor4To3<float>(ConvertColor255To01<float>(GetSpecular())));
        program->UniformVec3f("spotLight.position", position_);
        program->UniformVec3f("spotLight.direction", direction_);
        program->Uniform1f("spotLight.angle.outer_cutoff", outer_cutoff_);
        program->Uniform1f("spotLight.angle.inner_cutoff", inner_cutoff_);
    }
}

}
