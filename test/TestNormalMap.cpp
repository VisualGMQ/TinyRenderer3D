#include "test_framework.hpp"
#include "tinyrenderer3d/light.hpp"
#include "tinyrenderer3d/texture.hpp"
#include "tinyrenderer3d/vmath.hpp"

using namespace tinyrenderer3d;

class TestNormalMap: public TestFramework {
 public:
    TestNormalMap(): TestFramework("test normal map") {
        initPlane();

        GetRenderer()->AddObject(&plane_);

        initLight();
        GetRenderer()->SetLights(lights_);
    }

    ~TestNormalMap() {
    }

    void Step() override{
        static float angle = 0;
        static char sign = 1;
        plane_.SetRotation(angle, 0, 0);
        angle += sign;
        if (angle > 145) {
            sign *= -1;
            angle = 145;
        }
        if (angle < 45) {
            sign *= -1;
            angle = 45;
        }
    }

 private:
    Model plane_;
    LightSet lights_;

    void initLight() {
        lights_.dirlight.SetAmbient(0.5, 0.5, 0.5);
        lights_.dirlight.SetDirection(-1, -1, -1);
        lights_.dirlight.SetDiffuse(1, 1, 1);
        lights_.dirlight.SetSpecular(0.5, 0.5, 0.5);

        DotLight dotlight;
        dotlight.SetPosition(1, 1, 1);
        dotlight.SetAmbient(0, 0, 0);
        dotlight.SetDiffuse(0.55, 0.55, 0.55);
        dotlight.SetSpecular(0.55, 0.55, 0.55);
        lights_.dotlights.push_back(dotlight);
    }

    void initPlane() {
        plane_.Load("test_resources/brick/brick.obj");
        plane_.SetPosition(0, 0, 0);
        plane_.SetRotation(90, 0, 0);
    }
};

RUN_TEST(TestNormalMap)
