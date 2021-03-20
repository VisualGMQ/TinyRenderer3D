#include "app.hpp"
#include "tinyrenderer3d/light.hpp"
#include "tinyrenderer3d/material.hpp"

class DrawBasic: public App {
 public:
    DrawBasic() {
        texture_ = tinyrenderer3d::LoadTexture(render_, tinyrenderer3d::TEXTURE_TYPE_STATIC, "test_resources/ghost.png");

        tinyrenderer3d::DirectionLight dirlight;
        tinyrenderer3d::DotLight dotlight;
        tinyrenderer3d::SpotLight spotlight;

        dirlight.SetDirection(-2, -2, -2);
        dirlight.SetAmbient(0, 0, 0);
        dirlight.SetDiffuse(0, 0, 0);
        dirlight.SetSpecular(255, 255, 255);

        dotlight.SetAmbient(0, 0, 0);
        dotlight.SetDiffuse(255, 255, 255);
        dotlight.SetSpecular(255, 255, 255);
        dotlight.SetPosition(2, 2, 2);
        dotlight.SetParameter(1, 0.7, 0.18);

        spotlight.SetAmbient(0, 0, 0);
        spotlight.SetDiffuse(255, 255, 255);
        spotlight.SetSpecular(255, 255, 255);
        spotlight.SetPosition(0, 4, 0);
        spotlight.SetDirection(0, -1, 0);
        spotlight.SetParameter(30, 20);

        lights_.dirlight = dirlight;
        lights_.dotlights.push_back(dotlight);
        lights_.spotlights.push_back(spotlight);

        render_->SetLights(lights_);

        initColorCube();
        initTextureCube();
        initPlane();
        
        render_->AddObject(&color_cube_);
        render_->AddObject(&texture_cube_);
        render_->AddObject(&plane_);
    }

    virtual ~DrawBasic() {
        tinyrenderer3d::DestroyTexture(texture_);
    }

 private:
    tinyrenderer3d::Texture* texture_ = nullptr;
    tinyrenderer3d::LightSet lights_;
    tinyrenderer3d::Cube color_cube_;
    tinyrenderer3d::Cube texture_cube_;
    tinyrenderer3d::Plane plane_;

    void step() override {
        color_cube_.rotation.x += 2;
        color_cube_.rotation.z += 4;

        texture_cube_.rotation.x += 2;
        texture_cube_.rotation.z += 4;

        render_->Draw();
    }

    void initColorCube() {
        color_cube_.center = {-1.5, 0, 0};  // position
        color_cube_.l = 1;                  // length
        color_cube_.rotation = {0, 0, 0};   // rotation

        color_cube_.material = tinyrenderer3d::Material::CreateColorMaterial({0, 0, 0, 255}, {0, 200, 0, 255}, {0, 200, 0, 255}, 32);
    }

    void initTextureCube() {
        texture_cube_.center = {1.5, 0, 0};  // position
        texture_cube_.l = 1;                 // length
        texture_cube_.rotation = {0, 0, 0};  // rotation

        texture_cube_.material = tinyrenderer3d::Material::CreateTextureMaterial(texture_, {0, 200, 0, 255}, 32);
    }

    void initPlane() {
        plane_.center = {0, -1, 0};
        plane_.size = {10, 10};
        plane_.rotation = {0, 0, 0};

        plane_.material = tinyrenderer3d::Material::CreateTextureMaterial(texture_, {200, 200, 0, 255}, 32);
    }
};

int main(int argc, char** argv) {
    DrawBasic app;
    app.MainLoop();
    return 0;
}
