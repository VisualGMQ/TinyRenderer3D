#include "tinyrenderer3d/light.hpp"

namespace tinyrenderer3d {

void DirectionLight::Apply(Program* program, int idx) const {
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

void DotLight::Apply(Program* program, int idx) const {
    string light_name = "dotLights[" + std::to_string(idx) + "]";
    program->UniformVec3f(light_name+".base.ambient", ConvertColor4To3<float>(ConvertColor255To01<float>(GetAmbient())));
    program->UniformVec3f(light_name+".base.diffuse", ConvertColor4To3<float>(ConvertColor255To01<float>(GetDiffuse())));
    program->UniformVec3f(light_name+".base.specular", ConvertColor4To3<float>(ConvertColor255To01<float>(GetSpecular())));
    program->UniformVec3f(light_name+".position", position_);
    program->Uniform1f(light_name+".attenuation.constant", parameter_.constant);
    program->Uniform1f(light_name+".attenuation.linear", parameter_.linear);
    program->Uniform1f(light_name+".attenuation.quadratic", parameter_.quadratic);
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

void SpotLight::Apply(Program* program, int idx) const {
    if (program) {
        string light_name = "spotLights[" + std::to_string(idx) + "]";
        program->UniformVec3f(light_name+".base.ambient", ConvertColor4To3<float>(ConvertColor255To01<float>(GetAmbient())));
        program->UniformVec3f(light_name+".base.diffuse", ConvertColor4To3<float>(ConvertColor255To01<float>(GetDiffuse())));
        program->UniformVec3f(light_name+".base.specular", ConvertColor4To3<float>(ConvertColor255To01<float>(GetSpecular())));
        program->UniformVec3f(light_name+".position", position_);
        program->UniformVec3f(light_name+".direction", direction_);
        program->Uniform1f(light_name+".angle.outer_cutoff", outer_cutoff_);
        program->Uniform1f(light_name+".angle.inner_cutoff", inner_cutoff_);
    }
}

}
