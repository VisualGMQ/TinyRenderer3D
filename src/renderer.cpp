#include "tinyrenderer3d/renderer.hpp"
#include "stb_image.h"
#include "tinyrenderer3d/light.hpp"

namespace tinyrenderer3d {

Renderer::Renderer(int window_width, int window_height) {
    initShaders();
    initColors();
    initFeatures();
    initDrawSize(window_width, window_height);
    camera_ = new Camera;
    stbi_set_flip_vertically_on_load(true);
};

void Renderer::initShaders() {
    pure_color_program_ = CreateProgram("shader/pure_color_shader.vert", "shader/pure_color_shader.frag");
    texture_program_ = CreateProgram("shader/tex_shader.vert", "shader/tex_shader.frag");
}

void Renderer::initDrawSize(int w, int h) {
    window_size_.w = w;
    window_size_.h = h;
    SetViewport(0, 0, w, h);
    project_ = glm::perspective(glm::radians(45.0f), window_size_.w/static_cast<float>(window_size_.h), 0.1f, 100.0f);
}

void Renderer::initColors() {
    clear_color_.r = 0;
    clear_color_.g = 0;
    clear_color_.b = 0;
    clear_color_.a = 255;

    SetFillColorOpacity(255);
    SetDrawColorOpacity(255);
}

void Renderer::initFeatures() {
    GLCall(glEnable(GL_BLEND));
    GLCall(glEnable(GL_PROGRAM_POINT_SIZE));
    GLCall(glEnable(GL_MULTISAMPLE));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GLCall(glEnable(GL_STENCIL_TEST));
    GLCall(glEnable(GL_DEPTH_TEST));
    GLCall(glEnable(GL_CULL_FACE));
    GLCall(glCullFace(GL_BACK));
    GLCall(glFrontFace(GL_CCW));
}

void Renderer::SetClearColor(const Color& color) {
    SetClearColor(color.r, color.g, color.b, color.a);
}

void Renderer::SetClearColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    GLCall(glClearColor(r/255.0, g/255.0, b/255.0, a/255.0));
}

void Renderer::SetDrawColor(uint8_t r, uint8_t g, uint8_t b) {
    draw_color_.r = r;
    draw_color_.g = g;
    draw_color_.b = b;
}

void Renderer::SetDrawColorOpacity(uint8_t opacity) {
    draw_color_.a = opacity;
}

void Renderer::SetFillColor(uint8_t r, uint8_t g, uint8_t b) {
    fill_color_.r = r;
    fill_color_.g = g;
    fill_color_.b = b;
}

void Renderer::SetFillColorOpacity(uint8_t opacity) {
    fill_color_.a = opacity;
}

void Renderer::SetViewport(int x, int y, int w, int h) {
    GLCall(glViewport(0, 0, w, h));
}

void Renderer::Clear() {
    GLCall(glClear(GL_COLOR_BUFFER_BIT|GL_STENCIL_BUFFER_BIT|GL_DEPTH_BUFFER_BIT));
}

void Renderer::SetPointSize(uint32_t size) {
    GLCall(glPointSize(size));
}

void Renderer::AddObject(Drawable* obj) {
    drawables_.push_back(obj);
}

void Renderer::Draw() {
    for (Drawable* obj: drawables_) {
        drawOneObj(obj);
    }
}

void Renderer::drawOneObj(Drawable* obj) {
    Program* program = nullptr;
    if (obj->HasTexture()) {
        program = UseTextureProgram();
    } else {
        program = UsePureColorProgram();
    }
    applyMatrices(program, project_, camera_->GetMatrix());
    applyLights(program, lights_);

    obj->Draw(program);
}

void Renderer::applyMatrices(Program* program, const Mat4<float>& project, const Mat4<float>& view) {
    program->UniformMat4f("project", project_);
    program->UniformMat4f("view", camera_->GetMatrix());
    program->UniformVec3f("viewPos", camera_->GetPosition());
}

void Renderer::applyLights(Program* program, LightSet& lights) {
    program->Uniform1i("lightnum.dotlight", lights.dotlights.size());
    program->Uniform1i("lightnum.spotlight", lights.spotlights.size());

    lights.dirlight.Apply(pure_color_program_, 0);
    for (int i = 0; i < lights.dotlights.size(); i++) {
        lights.dotlights.at(i).Apply(program, i);
    }
    for (int i = 0; i < lights.spotlights.size(); i++) {
        lights.spotlights.at(i).Apply(program, i);
    }

}

void Renderer::SetTarget(Texture* texture) {
    if (texture) {
        is_at_default_framebuffer = false;
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, texture->fbo_));
        SetViewport(0, 0, texture->GetSize().w, texture->GetSize().h);
    } else {
        is_at_default_framebuffer = true;
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
        SetViewport(0, 0, window_size_.w, window_size_.h);
    }
}

void Renderer::EnablePolygonMode() {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}
void Renderer::DisablePolygonMode() {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

Renderer::~Renderer() {
    destroy();
}

void Renderer::destroy() {
    delete pure_color_program_;
    delete texture_program_;
    delete camera_;
}

Renderer* CreateRenderer(int window_width, int window_height) {
    Renderer* render = new Renderer(window_width, window_height);
    return render;
}

void DestroyRenderer(Renderer* render) {
    delete render;
}

};
