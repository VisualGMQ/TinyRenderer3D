#include "test_framework.hpp"
#include "tinyrenderer3d/light.hpp"
#include "tinyrenderer3d/texture.hpp"
#include "tinyrenderer3d/vmath.hpp"

using namespace tinyrenderer3d;

class TestShadow: public TestFramework {
 public:
    TestShadow(): TestFramework("test shadow") {
        texture_ = LoadTexture(TEXTURE_TYPE_STATIC, "test_resources/ghost.png");
        initPlane();
        initTextureCube();

        GetRenderer()->AddObject(&plane_);
        GetRenderer()->AddObject(&texture_cube_);

        initLight();
        GetRenderer()->SetLights(lights_);
    }

    ~TestShadow() {
        DestroyTexture(texture_);
    }

    void Step() override{
        auto render = GetRenderer();

        texture_cube_.rotation.x += 2;         
        texture_cube_.rotation.z += 3;         
    }

 private:
    Mesh plane_;
    Mesh texture_cube_;
    Texture* texture_;
    LightSet lights_;

    void initLight() {
        lights_.dirlight.SetAmbient(0.5, 0.5, 0.5);
        lights_.dirlight.SetDirection(0, -1, 0);
        lights_.dirlight.SetDiffuse(0.5, 0.5, 0.5);
        lights_.dirlight.SetSpecular(0.5, 0.5, 0.5);

        // DotLight dotlight;
        // dotlight.SetPosition(1, 1, 1);
        // dotlight.SetAmbient(0, 0, 0);
        // dotlight.SetDiffuse(0.55, 0.55, 0.55);
        // dotlight.SetSpecular(0.55, 0.55, 0.55);
        // lights_.dotlights.push_back(dotlight);

        // SpotLight spotlight;
        // spotlight.SetPosition(0, 1, 0);
        // spotlight.SetDirection(0, -1, 0);
        // spotlight.SetAmbient(0.8, 0.8, 0.8);
        // spotlight.SetDiffuse(0.8, 0.8, 0.8);
        // spotlight.SetSpecular(0.4, 0.4, 0.4);
        // spotlight.SetParameter(50, 40);
        // lights_.spotlights.push_back(spotlight);
    }

    void initPlane() {
        const vector<Vertex> vertices = {
            {{ 0.5f, 0.0f,  0.5f},  {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
            {{ 0.5f, 0.0f, -0.5f},  {1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
            {{-0.5f, 0.0f, -0.5f},  {0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
            {{-0.5f, 0.0f,  0.5f},  {0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        };

        const vector<uint> indices = {
            0, 1, 2,
            0, 2, 3
        };

        plane_.center = {0, -1, 0};
        plane_.vertices = vertices;
        plane_.indices = indices;
        plane_.scale = {10, 1, 10};
        plane_.material.ambient = {0.6, 0.6, 0.6};
        plane_.material.diffuse = {0.6, 0.6, 0.6};
        plane_.material.specular = {0.6, 0.6, 0.6};
        plane_.material.shininess = 32;
        // plane_.material.diffuse_texture = GetRenderer()->GetShadowMap();
    }

    void initTextureCube() {
        vector<Vertex> vertices = {
            {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},
            {{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}},
            {{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},
            {{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}},
            {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},
            {{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}, {0.0f, 0.0f, -1.0f}},

            {{-0.5f, -0.5f,  0.5f},  {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
            {{ 0.5f, -0.5f,  0.5f},  {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}}, 
            {{ 0.5f,  0.5f,  0.5f},  {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}}, 
            {{ 0.5f,  0.5f,  0.5f},  {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}}, 
            {{-0.5f,  0.5f,  0.5f},  {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
            {{-0.5f, -0.5f,  0.5f},  {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},

            {{-0.5f,  0.5f,  0.5f},  {1.0f, 0.0f}, {-1.0f, 0.0, 0.0f}},
            {{-0.5f,  0.5f, -0.5f},  {1.0f, 1.0f}, {-1.0f, 0.0, 0.0f}},
            {{-0.5f, -0.5f, -0.5f},  {0.0f, 1.0f}, {-1.0f, 0.0, 0.0f}},
            {{-0.5f, -0.5f, -0.5f},  {0.0f, 1.0f}, {-1.0f, 0.0, 0.0f}},
            {{-0.5f, -0.5f,  0.5f},  {0.0f, 0.0f}, {-1.0f, 0.0, 0.0f}},
            {{-0.5f,  0.5f,  0.5f},  {1.0f, 0.0f}, {-1.0f, 0.0, 0.0f}},

            {{ 0.5f,  0.5f,  0.5f},  {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}}, 
            {{ 0.5f, -0.5f, -0.5f},  {0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}}, 
            {{ 0.5f,  0.5f, -0.5f},  {1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}}, 
            {{ 0.5f, -0.5f, -0.5f},  {0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}}, 
            {{ 0.5f,  0.5f,  0.5f},  {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}}, 
            {{ 0.5f, -0.5f,  0.5f},  {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}}, 

            {{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}, {0.0f, -1.0f, 0.0f}},
            {{ 0.5f, -0.5f, -0.5f}, {1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}}, 
            {{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}}, 
            {{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}}, 
            {{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
            {{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}, {0.0f, -1.0f, 0.0f}},

            {{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
            {{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}}, 
            {{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}}, 
            {{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}}, 
            {{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
            {{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}}
        };

        texture_cube_.vertices = vertices;
        texture_cube_.center = {1.5, 0, 0};
        texture_cube_.scale = {1, 1, 1};

        texture_cube_.center = {0, 0, 0};

        texture_cube_.material.ambient = {0, 0.7, 0.7};
        texture_cube_.material.diffuse = {0, 0.7, 0.7};
        texture_cube_.material.specular = {0, 0.7, 0.7};
        texture_cube_.material.shininess = 32;
        texture_cube_.material.diffuse_texture = texture_;
    }
};

RUN_TEST(TestShadow)
