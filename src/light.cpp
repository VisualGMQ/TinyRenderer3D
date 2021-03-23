#include "tinyrenderer3d/light.hpp"

namespace tinyrenderer3d {

void DirectionLight::Apply(Program* program, int idx) const {
    if (program) {
        UniformLightMatrix(program);
        program->UniformVec3f("directionLight.base.ambient", GetAmbient());
        program->UniformVec3f("directionLight.base.diffuse", GetDiffuse());
        program->UniformVec3f("directionLight.base.specular", GetSpecular());
        program->UniformVec3f("directionLight.direction", direction_);
    }
}

void DirectionLight::UniformLightMatrix(Program* program) const {
    Mat4<float> lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 7.5f);
    Vec3<float> up = {0, 1, 0};
    if (GetDirection() == Vec3<float>(0, 1, 0) || GetDirection() == Vec3<float>(0, -1, 0)) {
        up = {1, 0, 0};
    }
    Mat4<float> lightView = glm::lookAt(-GetDirection()*Vec3<float>(4, 4, 4), Vec3<float>(0.0f, 0.0f, 0.0f), up);
    program->UniformMat4f("lightmatrix", lightProjection*lightView);
}

void DotLight::SetParameter(float constant, float linear, float quadratic) {
    parameter_.constant = constant;
    parameter_.linear = linear;
    parameter_.quadratic = quadratic;
}

void DotLight::Apply(Program* program, int idx) const {
    string light_name = "dotLights[" + std::to_string(idx) + "]";
    program->UniformVec3f(light_name+".base.ambient",  GetAmbient());
    program->UniformVec3f(light_name+".base.diffuse",  GetDiffuse());
    program->UniformVec3f(light_name+".base.specular", GetSpecular());
    program->UniformVec3f(light_name+".position", position_);
    program->Uniform1f(light_name+".attenuation.constant", parameter_.constant);
    program->Uniform1f(light_name+".attenuation.linear", parameter_.linear);
    program->Uniform1f(light_name+".attenuation.quadratic", parameter_.quadratic);
}

// TODO not finish
void DotLight::UniformLightMatrix(Program* program) const {}

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
        program->UniformVec3f(light_name+".base.ambient",  GetAmbient());
        program->UniformVec3f(light_name+".base.diffuse",  GetDiffuse());
        program->UniformVec3f(light_name+".base.specular", GetSpecular());
        program->UniformVec3f(light_name+".position", position_);
        program->UniformVec3f(light_name+".direction", direction_);
        program->Uniform1f(light_name+".angle.outer_cutoff", outer_cutoff_);
        program->Uniform1f(light_name+".angle.inner_cutoff", inner_cutoff_);
    }
}

// TODO not finish
void SpotLight::UniformLightMatrix(Program* program) const {}

}
