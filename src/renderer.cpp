#include "tinyrenderer3d/renderer.hpp"
#include "stb_image.h"
#include "tinyrenderer3d/program.hpp"

namespace tinyrenderer3d {

Renderer::Renderer(int window_width, int window_height) {
    initShaders();
    initColors();
    initFeatures();
    initDrawSize(window_width, window_height);

    // TODO if shadow_map_'s size are (window_width, window_height)???
    shadow_map_ = new ShadowMap(1024, 1024);
    camera_ = new Camera;

    stbi_set_flip_vertically_on_load(true);
};

void Renderer::initShaders() {
    texture_program_ = CreateProgram(TextureProgramName, "shader/tex_shader.vert", "shader/tex_shader.frag");
    shadow_program_ = CreateProgram(ShadowProgramName, "shader/shadow.vert", "shader/empty.frag");
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
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GLCall(glEnable(GL_PROGRAM_POINT_SIZE));
    GLCall(glEnable(GL_STENCIL_TEST));
    GLCall(glEnable(GL_DEPTH_TEST));
    EnableMultisample();
    EnableFaceCull(GL_BACK, true);
}

void Renderer::EnableMultisample() {
    GLCall(glEnable(GL_MULTISAMPLE));
}

void Renderer::DisableMultisample() {
    GLCall(glDisable(GL_MULTISAMPLE));
}

void Renderer::EnableFaceCull(GLenum face, bool front_ccw) {
    GLCall(glEnable(GL_CULL_FACE));
    GLCall(glCullFace(face));
    if (front_ccw) {
        GLCall(glFrontFace(GL_CCW));
    } else {
        GLCall(glFrontFace(GL_CW));
    }
}

void Renderer::DisableFaceCull() {
    GLCall(glDisable(GL_CULL_FACE));
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

void Renderer::DrawShadowPre() {
    // TODO when you want use shadow, open it
    // GLint data[4] = {0};
    // GLCall(glGetIntegerv(GL_VIEWPORT, data));
    // Rect<int> viewport = {{data[0], data[1]}, {data[2], data[3]}};
    // Program* program = UseShadowProgram();
    // // TODO add more light uniform matrix
    // lights_.dirlight.UniformLightMatrix(program);
    // // TODO uniform lightmatrix to program
    // shadow_map_->Use(viewport);
    // // TODO uniform lightmatrix to program
    for (Drawable* obj: drawables_) {
        obj->DrawForShadow(program);
    }
    // shadow_map_->DontUse();
}

void Renderer::Draw() {
    // DrawShadowPre();
    GLCall(glActiveTexture(GL_TEXTURE6));
    shadow_map_->UseAsTarget();
    Program* program = UseTextureProgram();
    // if you want to use shadow, uncomment it
    // program->Uniform1i("shadow_texture", 6);
    for (Drawable* obj: drawables_) {
        drawOneObj(obj, program);
    }
}

void Renderer::drawOneObj(Drawable* obj, Program* program) {
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

    lights.dirlight.Apply(program, 0);
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
    DestroyProgram(texture_program_);
    DestroyProgram(shadow_program_);
    delete shadow_map_;
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
