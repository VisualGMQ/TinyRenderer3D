#include "test_framework.hpp"
#include "tinyrenderer3d/light.hpp"
#include "tinyrenderer3d/texture.hpp"
#include "tinyrenderer3d/vmath.hpp"

using namespace tinyrenderer3d;

class DrawModel: public TestFramework {
 public:
    DrawModel():TestFramework("draw model") {
        initCubeModel();
        initNanoModel();
        auto render = GetRenderer();
        render->AddObject(&cube_);
        render->AddObject(&nano_);

        LightSet lights;
        lights.dirlight.SetDirection(-1, -1, -1);
        lights.dirlight.SetAmbient(0.5, 0.5, 0.5);
        lights.dirlight.SetDiffuse(1, 1, 1);
        lights.dirlight.SetSpecular(0.5, 0.5, 0.5);

        render->SetLights(lights);
    }

    ~DrawModel() {
    }

    void Step() override{
        static float angle = 0;
        cube_.SetRotation(0, angle, angle);
        angle += 2;
    }

 private:
    Model cube_;
    Model nano_;

    void initCubeModel() {
        cube_.Load("test_resources/cube/cube.obj");
        cube_.SetPosition(-1, 0, 0);
        cube_.SetScale(1, 1, 1);
    }

    void initNanoModel() {
        nano_.Load("test_resources/nanosuit/nanosuit.obj");
        nano_.SetPosition(2, -10, 0);
        nano_.SetScale(1, 1, 1);
    }
};

RUN_TEST(DrawModel)
