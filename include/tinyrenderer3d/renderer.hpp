#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <vector>
#include <array>
#include <cmath>

#define GLEW_STATIC
#include "GL/glew.h"

#include "types.hpp"
#include "vmath.hpp"
#include "program.hpp"
#include "tool.hpp"
#include "texture.hpp"
#include "log.hpp"
#include "camera.hpp"
#include "light.hpp"
#include "model.hpp"
#include "shadow_map.hpp"
#include "skybox.hpp"

namespace tinyrenderer3d {

class Mesh;

using std::vector;

class Renderer final {
 public:
    Renderer(int window_width, int window_height);
    ~Renderer();

    // some setter and getter function
    Color GetClearColor() const { return clear_color_; }
    void SetClearColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
    void SetClearColor(const Color& color);

    Color GetDrawColor() const { return draw_color_; }
    void SetDrawColor(uint8_t r, uint8_t g, uint8_t b);
    void SetDrawColorOpacity(uint8_t opacity);

    Color GetFillColor() const {return fill_color_; }
    void SetFillColor(uint8_t r, uint8_t g, uint8_t b);
    void SetFillColorOpacity(uint8_t opacity);

    void SetViewport(int x, int y, int w, int h);

    void SetPointSize(uint32_t size);

    // feature functions
    void EnableFaceCull(GLenum face, bool ccw);
    void DisableFaceCull();

    void EnableMultisample();
    void DisableMultisample();

    // camera functions
    Camera* GetCamera() { return camera_; }

    // program functions
    Program* UseShadowProgram() {
        shadow_program_->Use();
        return shadow_program_;
    }

    Program* UseTextureProgram() {
        texture_program_->Use();
        return texture_program_;
    }

    // some matrix related function
    Mat4<float> GetProjectMatrix() const {
        return project_;
    }
    
    // some draw function
    void AddObject(Drawable* obj);

    void DrawShadowPre();
    void Draw();

    // some light function
    void SetLights(LightSet& lights) { lights_ = lights; }
    LightSet GetLights() const { return lights_; }

    // some other functions
    void Clear();
    void SetTarget(Texture* texture);
    void EnablePolygonMode();
    void DisablePolygonMode();
    void SetSkyBox(SkyBox* skybox) { skybox_ = skybox; }
    void RemoveSkyBox() { skybox_ = nullptr; }

 private:
    glm::mat4 project_ = glm::mat4(1.0f);
    bool is_at_default_framebuffer = true;

    Color draw_color_;
    Color fill_color_;
    Color clear_color_;

    Size2d<i32> window_size_;

    Camera* camera_;
    LightSet lights_;
    vector<Drawable*> drawables_;

    Program* texture_program_ = nullptr;
    Program* shadow_program_ = nullptr;
    Program* skybox_program_ = nullptr;

    ShadowMap* shadow_map_ = nullptr;
    SkyBox* skybox_ = nullptr;

    void unbindBuffers() {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void initShaders();
    void initDrawSize(int w, int h);
    void initColors();
    void initFeatures();
    void initMatrices();

    void drawOneObj(Drawable*, Program*);
    void drawSkyBox();

    void applyLights(Program* program, LightSet& lights);
    void applyMatrices(Program* program, const Mat4<float>& project);

    void destroy();
};

Renderer* CreateRenderer(int window_width, int window_height);
void DestroyRenderer(Renderer* render);

}; // NAMESPACE tinyrender3d

#endif
