#ifndef TINYRENDERER3D_LIGHT_HPP
#define TINYRENDERER3D_LIGHT_HPP

#include <vector>

#include "vmath.hpp"
#include "program.hpp"

namespace tinyrenderer3d {

using std::vector;

// NOTE: when you change these value, don't forget to change them in shaders(if you changed shaders, modify these too)
constexpr int DotLightMaxNum = 15;
constexpr int SpotLightMaxNum = 15;

class ILight {
 public:
    virtual ~ILight() = default;

    Color3<float> GetAmbient() const { return ambient_; }
    void SetAmbient(float r, float g, float b) {
        ambient_.r = r;
        ambient_.g = g;
        ambient_.b = b;
    }

    Color3<float> GetDiffuse() const { return diffuse_; }
    void SetDiffuse(float r, float g, float b) {
        diffuse_.r = r;
        diffuse_.g = g;
        diffuse_.b = b;
    }

    Color3<float> GetSpecular() const { return specular_; }
    void SetSpecular(float r, float g, float b) {
        specular_.r = r;
        specular_.g = g;
        specular_.b = b;
    }

    virtual void UniformLightMatrix(Program* program) const = 0;
    virtual void Apply(Program* program, int idx) const = 0;

 private:
    Color3<float> ambient_ = {1, 1, 1};
    Color3<float> diffuse_ = {0, 0, 0};
    Color3<float> specular_ = {0, 0, 0};
};

class DirectionLight: public ILight {
 public:
    virtual ~DirectionLight() = default;

    Vec3<float> GetDirection() const { return direction_; }
    void SetDirection(float x, float y, float z) {
        direction_.x = x;
        direction_.y = y;
        direction_.z = z;
    }

    void UniformLightMatrix(Program* program) const override;
    void Apply(Program* program, int idx) const override;

 private:
    Vec3<float> direction_ = {0, -1, -1};
};

class DotLight: public ILight {
 public:
    virtual ~DotLight() = default;

    struct Parameter {
        float constant = 1;
        float linear = 0;
        float quadratic = 0;
    };
    void SetParameter(float constant, float linear, float quadratic);
    Parameter GetParameter() const { return parameter_; }

    void SetPosition(float x, float y, float z) {
        position_.x = x;
        position_.y = y;
        position_.z = z;
    }
    Vec3<float> GetPosition() const { return position_; }

    void UniformLightMatrix(Program* program) const override;
    void Apply(Program* program, int idx) const override;

 private:
    Vec3<float> position_ = {0, 0, 0};
    Parameter parameter_;
};

class SpotLight: public ILight {
 public:
    virtual ~SpotLight() = default;

    void SetParameter(float outer_cutoff, float inner_cutoff);
    void SetDirection(float x, float y, float z);
    void SetPosition(float x, float y, float z);
    Vec3<float> GetPosition() const { return position_; }
    Vec3<float> GetDirection() const { return direction_; }
    float GetOuterCutOff() const { return outer_cutoff_; }
    float GetInnerCutOff() const { return inner_cutoff_; }

    void UniformLightMatrix(Program* program) const override;
    void Apply(Program* program, int idx) const override;

 private:
    float outer_cutoff_ = 0;
    float inner_cutoff_ = 0;

    Vec3<float> direction_ = {0, 0, 0};
    Vec3<float> position_ = {0, 0, 0};
};

struct LightSet {
    DirectionLight dirlight;
    vector<DotLight> dotlights;
    vector<SpotLight> spotlights;
};

}

#endif
