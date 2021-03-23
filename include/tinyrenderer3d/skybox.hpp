#ifndef TINYRENDERER3D_SKYBOX_HPP
#define TINYRENDERER3D_SKYBOX_HPP

#include "cube_map.hpp"
#include "drawable.hpp"

namespace tinyrenderer3d {

class SkyBox: public Drawable {
 public:
    SkyBox(CubeMap* cubemap);
    void Draw(Program*) override;
    void DrawForShadow(Program* program) override {
        Log("skybox can't draw for shadow, don't call this function");
    }
    ~SkyBox();

 private:
    GLuint vbo_ = 0;
    GLuint vao_ = 0;
    CubeMap* map_ = nullptr;

    void bufferData();
    void bind();
    void unbind();
};

SkyBox* CreateSkyBox(CubeMap* cubmap);
void DestroySkyBox(SkyBox* box);

}

#endif
