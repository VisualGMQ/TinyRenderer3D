#include "app.hpp"
#include "tinyrenderer3d/light.hpp"

class DrawBasic: public App {
 public:
    DrawBasic() {
        texture_ = tinyrenderer3d::LoadTexture(render_, tinyrenderer3d::TEXTURE_TYPE_STATIC, "test_resources/ghost.png");
        // dirlight.SetAmbient(255, 255, 255);
        // dirlight.SetDiffuse(255, 255, 255);
        // dirlight.SetSpecular(255, 255, 255);
        dirlight.SetDirection(-2, -2, -2);
        dirlight.SetAmbient(0, 0, 0);
        dirlight.SetDiffuse(0, 0, 0);
        dirlight.SetSpecular(0, 0, 0);

        dotlight.SetAmbient(255, 255, 255);
        dotlight.SetDiffuse(255, 255, 255);
        dotlight.SetSpecular(255, 255, 255);
        dotlight.SetPosition(2, 2, 2);
        dotlight.SetParameter(1, 0.7, 0.18);

        spotlight.SetAmbient(255, 255, 255);
        spotlight.SetDiffuse(255, 255, 255);
        spotlight.SetSpecular(255, 255, 255);
        spotlight.SetPosition(0, 0, 1);
        spotlight.SetDirection(0, 0, -1);
        spotlight.SetParameter(50, 40);
    }

    virtual ~DrawBasic() {
        tinyrenderer3d::DestroyTexture(texture_);
    }

 private:
    tinyrenderer3d::Texture* texture_ = nullptr;
    tinyrenderer3d::DirectionLight dirlight;
    tinyrenderer3d::DotLight dotlight;
    tinyrenderer3d::SpotLight spotlight;

    void step() override {
        drawColorCube();
        drawTextureCube();
    }

    void drawColorCube() {
        static tinyrenderer3d::Cube cube= {
            {-1.5, 0, 0},  // position
            1,          // length
            {0, 0, 0}   // rotation
        };
        static tinyrenderer3d::Material material;

        material.ambient = {0, 0, 0, 255};
        material.diffuse = {0, 200, 0, 255};
        material.specular = {0, 200, 0, 255};
        material.shininess = 32;

        cube.rotation.x += 2;
        cube.rotation.z += 4;


        render_->DrawCube(cube, material, dirlight, dotlight, spotlight);
    }

    void drawTextureCube() {
        static tinyrenderer3d::Cube cube= {
            {1.5, 0, 0},  // position
            1,          // length
            {0, 0, 0}   // rotation
        };
        static tinyrenderer3d::Material material;

        material.ambient = {0, 0, 0, 255};
        material.specular = {0, 200, 0, 255};
        material.textures.push_back(texture_);
        material.shininess = 32;

        cube.rotation.x += 2;
        cube.rotation.z += 4;

        render_->DrawCube(cube, material, dirlight, dotlight, spotlight);
    }


};

int main(int argc, char** argv) {
    DrawBasic app;
    app.MainLoop();
    return 0;
}
