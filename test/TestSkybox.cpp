#include "test_framework.hpp"
#include "tinyrenderer3d/light.hpp"
#include "tinyrenderer3d/renderer.hpp"

using namespace tinyrenderer3d;

class TestSkybox: public TestFramework {
 public:
    TestSkybox():TestFramework("Test Skybox") {
        initSkyBox();
        initModel();

        LightSet lights;
        lights.dirlight.SetDirection(-1, -1, -1);
        lights.dirlight.SetAmbient(0.5, 0.5, 0.5);
        lights.dirlight.SetDiffuse(0.5, 0.5, 0.5);
        GetRenderer()->SetLights(lights);

        GetRenderer()->AddObject(&model_);

        GetRenderer()->SetSkyBox(skybox_);
    }

    ~TestSkybox() {
        DestroyCubeMap(cubmap_);
        delete skybox_;
    }

 private:
    SkyBox* skybox_ = nullptr;
    CubeMap* cubmap_ = nullptr;
    Model model_;

    void initSkyBox() {
        CubeMap::CubeMapTextureDatas datas;
        datas.negative_x = LoadImage("test_resources/skybox/left.jpg");
        datas.negative_y = LoadImage("test_resources/skybox/bottom.jpg");
        datas.negative_z = LoadImage("test_resources/skybox/back.jpg");
        datas.positive_x = LoadImage("test_resources/skybox/right.jpg");
        datas.positive_y = LoadImage("test_resources/skybox/top.jpg");
        datas.positive_z = LoadImage("test_resources/skybox/front.jpg");

        cubmap_ = CreateCubeMap(datas);
        skybox_ = new SkyBox(cubmap_);

        DestroyImage(datas.negative_x);
        DestroyImage(datas.negative_y);
        DestroyImage(datas.negative_z);
        DestroyImage(datas.positive_x);
        DestroyImage(datas.positive_y);
        DestroyImage(datas.positive_z);
    }

    void initModel() {
        model_.Load("test_resources/cube/cube.obj");
        model_.SetPosition(0, 0, 0);
    }
};

RUN_TEST(TestSkybox)
