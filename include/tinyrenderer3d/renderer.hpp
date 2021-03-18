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
#include "material.hpp"
#include "light.hpp"

namespace tinyrenderer3d {

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

    // camera functions
    Camera* GetCamera() { return camera_; }

    // program functions
    Program* UsePureColorProgram() {
        pure_color_program_->Use();
        return pure_color_program_;
    }

    Program* UseColorfulProgram() {
        // geo_colorful_program_->UseProgram(nullptr);
        return nullptr;
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
    void DrawCube(const Cube& cube, const Material& material, const DirectionLight& dirlight, const vector<DotLight*>& dotlights, const vector<SpotLight*>& spotlights);
    void DrawPlane(const Plane& plane, const Material& material, const DirectionLight& dirlight, const vector<DotLight*>& dotlights, const vector<SpotLight*>& spotlights);
    // void DrawLine(int x1, int y1, int x2, int y2);
    // void DrawLine(const Point& p1, const Point& p2);
    // void DrawLine(const ColorfulPoint& p1, const ColorfulPoint& p2);

    // void DrawLines(const Point* points, int num);
    // void DrawLines(const vector<Point>& points);
    // void DrawLines(const ColorfulPoint* points, int num);
    // void DrawLines(const vector<ColorfulPoint>& points);

    // void DrawRect(int x, int y, int w, int h);
    // void DrawRect(const Rect& rect);
    // 
    // void DrawPolygon(Point* points, int num);
    // void DrawPolygon(vector<Point>& points);
    // void DrawPolygon(ColorfulPoint* points, int num);
    // void DrawPolygon(vector<ColorfulPoint>& points);

    // void DrawCircle(int x, int y, int radius);

    // void DrawPoint(int x, int y);

    // void DrawTexture(Texture* texture, const Rect* src_rect, const Rect* dst_rect, const Color* color = nullptr, float degree = 0, FlipType flip = FLIP_NONE);

    // some other functions
    void Clear();
    void SetTarget(Texture* texture);
    void EnablePolygonMode();
    void DisablePolygonMode();

 private:
    GLuint vbo_ = 0;
    GLuint ebo_ = 0;
    GLuint vao_ = 0;

    glm::mat4 project_ = glm::mat4(1.0f);
    bool is_at_default_framebuffer = true;

    Color draw_color_;
    Color fill_color_;
    Color clear_color_;

    Size2d<i32> window_size_;

    Camera* camera_;

    Program* pure_color_program_ = nullptr;
    // Program* geo_colorful_program_ = nullptr;
    Program* texture_program_ = nullptr;

    void bindBuffers() {
        glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
        glBindVertexArray(vao_);
    }

    void unbindBuffers() {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void initBuffers();
    void initShaders();
    void initDrawSize(int w, int h);
    void initColors();
    void initFeatures();
    void initMatrices();

    void bufferData(GLenum buffer_type, const GLvoid* data, GLsizeiptr size, GLenum usage) {
        glBufferData(buffer_type, size, data, usage);
    }

    void tellVertexAttribPoint(GLuint index, GLint size, GLenum type, bool normalized, GLsizei stride, const GLvoid* offset) {
        glVertexAttribPointer(index, size, type, normalized, stride, offset);
        glEnableVertexAttribArray(index);
    }

    void applyPureColorProgram(const Mat4<float>& project, const Mat4<float>& model, const Mat4<float>& view,const Material& material, const DirectionLight& dirlight, const vector<DotLight*>& dotlights, const vector<SpotLight*>& spotlights);
    void applyTextureProgram(const Mat4<float>& project, const Mat4<float>& model, const Mat4<float>& view, const Material& material, const DirectionLight& dirlight, const vector<DotLight*>& dotlights, const vector<SpotLight*>& spotlights);
    void applyLights(Program* program, const Material& material, const DirectionLight& dirlight, const vector<DotLight*>& dotlights, const vector<SpotLight*>& spotlights);
    void applyMatrices(Program* program, const Mat4<float>& project, const Mat4<float>& model, const Mat4<float>& view);

    void destroy();
};

Renderer* CreateRenderer(int window_width, int window_height);
void DestroyRenderer(Renderer* render);

}; // NAMESPACE tinyrender3d

#endif
