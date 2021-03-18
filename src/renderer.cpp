#include "tinyrenderer3d/renderer.hpp"
#include "stb_image.h"
#include "tinyrenderer3d/light.hpp"

namespace tinyrenderer3d {

Renderer::Renderer(int window_width, int window_height) {
    initBuffers();
    initShaders();
    initColors();
    initFeatures();
    initDrawSize(window_width, window_height);
    camera_ = new Camera;
    stbi_set_flip_vertically_on_load(true);
};

void Renderer::initBuffers() {
    GLCall(glGenBuffers(1, &vbo_));
    GLCall(glGenBuffers(1, &ebo_));
    GLCall(glGenVertexArrays(1, &vao_));
}

void Renderer::initShaders() {
    pure_color_program_ = CreateProgram("shader/pure_color_shader.vert", "shader/pure_color_shader.frag");
    // geo_colorful_program_ = CreateProgram("shader/geo_colorful_shader.vert", "shader/geo_colorful_shader.frag");
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

void Renderer::DrawCube(const Cube& cube, const Material& material, const DirectionLight& dirlight, const DotLight& dotlight, const SpotLight& spotlight) {
    static float data[] = {
        // position           texcoord     normal
        // Back face
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0, 0, -1,    // Bottom-left
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0, 0, -1,    // top-right
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0, 0, -1,    // bottom-right         
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0, 0, -1,    // top-right
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0, 0, -1,    // bottom-left
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0, 0, -1,    // top-left
        // Front face
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0, 0, 1,     // bottom-left
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0, 0, 1,     // bottom-right
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0, 0, 1,     // top-right
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0, 0, 1,     // top-right
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0, 0, 1,     // top-left
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0, 0, 1,     // bottom-left
        // Left face
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  -1, 0, 0,    // top-right
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  -1, 0, 0,    // top-left
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  -1, 0, 0,    // bottom-left
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  -1, 0, 0,    // bottom-left
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  -1, 0, 0,    // bottom-right
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  -1, 0, 0,    // top-right
        // Right face
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1, 0, 0,     // top-left
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1, 0, 0,     // bottom-right
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1, 0, 0,     // top-right         
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1, 0, 0,     // bottom-right
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1, 0, 0,     // top-left
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1, 0, 0,     // bottom-left     
        // Bottom face
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0, -1, 0,    // top-right
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  0, -1, 0,    // top-left
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0, -1, 0,    // bottom-left
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0, -1, 0,    // bottom-left
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0, -1, 0,    // bottom-right
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0, -1, 0,    // top-right
        // Top face
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0, 1, 0,     // top-left
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0, 1, 0,     // bottom-right
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0, 1, 0,     // top-right     
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0, 1, 0,     // bottom-right
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0, 1, 0,     // top-left
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  0, 1, 0      // bottom-left
    };

    Mat4<float> model = Mat4<float>(1.0f);
    model = glm::translate(model, cube.center);
    model = model*GetRotateMatByQuat(cube.rotation);
    model = glm::scale(model, Vec3<float>(cube.l));

    bindBuffers();

    bufferData(GL_ARRAY_BUFFER, data, sizeof(data), GL_STATIC_DRAW);

    Program* program = nullptr;
    if (material.textures.empty()) {   // use pure color program
        tellVertexAttribPoint(0, 3, GL_FLOAT, false, 8*sizeof(float), 0);
        tellVertexAttribPoint(1, 3, GL_FLOAT, false, 8*sizeof(float), (void*)(5*sizeof(float)));
        program = UsePureColorProgram();
        program->UniformVec3f("material.diffuse", ConvertColor4To3<float>(ConvertColor255To01<float>(material.diffuse)));
    } else {    // use texture program
        tellVertexAttribPoint(0, 3, GL_FLOAT, false, 8*sizeof(float), 0);
        tellVertexAttribPoint(1, 2, GL_FLOAT, false, 8*sizeof(float), (void*)(3*sizeof(float)));
        tellVertexAttribPoint(2, 3, GL_FLOAT, false, 8*sizeof(float), (void*)(5*sizeof(float)));

        program = UseTextureProgram();
        GLCall(glBindTexture(GL_TEXTURE_2D, material.textures.at(0)->tex_));
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, material.textures.at(0)->fbo_));
        GLCall(glActiveTexture(GL_TEXTURE0));
        program->Uniform1i("material.diffuse", 0);
    }
    program->UniformMat4f("project", project_);
    program->UniformMat4f("model", model);
    program->UniformMat4f("view", camera_->GetMatrix());

    auto color = ConvertColor4To3<float>(ConvertColor255To01<float>(material.ambient));
    program->UniformVec3f("material.ambient", color);
    program->UniformVec3f("material.specular", ConvertColor4To3<float>(ConvertColor255To01<float>(material.specular)));
    program->Uniform1f("material.shininess", material.shininess);

    dirlight.Apply(program);
    dotlight.Apply(program);
    spotlight.Apply(program);

    program->UniformVec3f("viewPos", camera_->GetPosition());

    GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));

    unbindBuffers();
}

// void Renderer::DrawLine(int x1, int y1, int x2, int y2) {
//     Point data[2];
//     data[0].x = x1;
//     data[0].y = y1;
//     data[1].x = x2;
//     data[1].y = y2;
// 
//     DrawLines(data, 2);
// }
// 
// void Renderer::DrawLine(const Point& p1, const Point& p2) {
//     DrawLine(p1.x, p1.y, p2.x, p2.y);
// }
// 
// void Renderer::DrawLines(const vector<Point>& points) {
//     DrawLines(points.data(), points.size());
// }
// 
// void Renderer::DrawLines(const Point* points, int num) {
//     useGeomentryPureColorProgram();
//     geo_pure_color_program_->UniformMat4f("proj", current_proj_);
//     geo_pure_color_program_->UniformVec4i("fragColor256", draw_color_);
// 
//     bindBuffers(0);
//     glBufferData(GL_ARRAY_BUFFER, num*sizeof(Point), points, GL_STATIC_DRAW);
//     glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Point), (void*)0);
//     glEnableVertexAttribArray(0);
//     
//     glDrawArrays(GL_LINE_STRIP, 0, num);
// }
// 
// void Renderer::DrawLine(const ColorfulPoint& p1, const ColorfulPoint& p2) {
//     ColorfulPoint points[] = {p1, p2};
//     DrawLines(points, 2);
// }
// 
// void Renderer::DrawLines(const vector<ColorfulPoint>& points) {
//     DrawLines(points.data(), points.size());
// }
// 
// void Renderer::DrawLines(const ColorfulPoint* points, int num) {
//     if (num < 2)
//         return;
//     useGeomentryColorfulProgram();
//     geo_colorful_program_->UniformMat4f("proj", current_proj_);
// 
//     bindBuffers(0);
//     glBufferData(GL_ARRAY_BUFFER, num*sizeof(ColorfulPoint), points, GL_STATIC_DRAW);
//     glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(ColorfulPoint), (void*)offsetof(ColorfulPoint, point));
//     glEnableVertexAttribArray(0);
//     glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(ColorfulPoint), (void*)offsetof(ColorfulPoint, color));
//     glEnableVertexAttribArray(1);
//     
//     glDrawArrays(GL_LINE_STRIP, 0, num);
// }
// 
// void Renderer::DrawRect(int x, int y, int w, int h) {
//     Point points[4];
//     points[0].x = x;
//     points[0].y = y;
//     points[1].x = x+w;
//     points[1].y = y;
//     points[2].x = x+w;
//     points[2].y = y+h;
//     points[3].x = x;
//     points[3].y = y+h;
// 
//     DrawPolygon(points, 4);
// }
// 
// void Renderer::DrawRect(const Rect& rect) {
//     DrawRect(rect.x, rect.y, rect.w, rect.h);
// }
// 
// void Renderer::DrawPolygon(vector<Point>& points) {
//     DrawPolygon(points.data(), points.size());
// }
// 
// void Renderer::DrawPolygon(Point* points, int num) {
//     if (num <= 2)
//         return;
// 
//     useGeomentryPureColorProgram();
// 
//     bindBuffers(0);
//     geo_pure_color_program_->UniformMat4f("proj", current_proj_);
//     glBufferData(GL_ARRAY_BUFFER, sizeof(Point)*num, points, GL_STATIC_DRAW);
//     glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Point), (void*)0);
//     glEnableVertexAttribArray(0);
// 
//     // fill polygon
//     if (fill_color_.a != 0) {
//         geo_pure_color_program_->UniformVec4i("fragColor256", fill_color_);
//         glDrawArrays(GL_TRIANGLE_FAN, 0, num);
//     }
//     
//     // draw outline
//     geo_pure_color_program_->UniformVec4i("fragColor256", draw_color_);
//     glDrawArrays(GL_LINE_LOOP, 0, num);
// }
// 
// 
// void Renderer::DrawPolygon(vector<ColorfulPoint>& points) {
//     DrawPolygon(points.data(), points.size());
// }
// 
// void Renderer::DrawPolygon(ColorfulPoint* points, int num) {
//     if (num <= 2)
//         return;
//     useGeomentryColorfulProgram();
//     geo_colorful_program_->UniformMat4f("proj", current_proj_);
// 
//     bindBuffers(0);
//     glBufferData(GL_ARRAY_BUFFER, sizeof(ColorfulPoint)*num, points, GL_STATIC_DRAW);
//     glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(ColorfulPoint), (void*)offsetof(ColorfulPoint, point));
//     glEnableVertexAttribArray(0);
//     glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(ColorfulPoint), (void*)offsetof(ColorfulPoint, color));
//     glEnableVertexAttribArray(1);
// 
//     glDrawArrays(GL_TRIANGLE_FAN, 0, num);
// }
// 
// void Renderer::DrawCircle(int x, int y, int radius) {
//     float delta_radian = 2*glm::pi<float>()/100;
//     vector<Point> points;
// 
//     float radian = 0;
//     while (radian <= 2*glm::pi<float>()) {
//         Point p;
//         p.x = x+cos(radian)*radius;
//         p.y = y+sin(radian)*radius;
//         points.push_back(p);
//         radian += delta_radian;
//     }
// 
//     useGeomentryPureColorProgram();
// 
//     geo_pure_color_program_->UniformMat4f("proj", current_proj_);
// 
//     bindBuffers(0);
//     glBufferData(GL_ARRAY_BUFFER, sizeof(Point)*points.size(), points.data(), GL_STATIC_DRAW);
//     glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Point), (void*)0);
//     glEnableVertexAttribArray(0);
// 
//     // fill circle
//     if (fill_color_.a != 0) {
//         geo_pure_color_program_->UniformVec4i("fragColor256", fill_color_);
//         glDrawArrays(GL_TRIANGLE_FAN, 0, points.size());
//     }
//     
//     // draw outline
//     geo_pure_color_program_->UniformVec4i("fragColor256", draw_color_);
//     glDrawArrays(GL_LINE_LOOP, 0, points.size());
// }
// 
// // FIXME can't work
// void Renderer::DrawPoint(int x, int y) {
//     float data[2];
//     data[0] = x;
//     data[1] = y;
// 
//     useGeomentryPureColorProgram();
//     bindBuffers(0);
//     glBufferData(GL_ARRAY_BUFFER, 2*sizeof(float), data, GL_STATIC_DRAW);
//     glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
//     glEnableVertexAttribArray(0);
// 
//     geo_pure_color_program_->UniformMat4f("proj", current_proj_);
//     geo_pure_color_program_->UniformVec4i("fragColor256", draw_color_);
// 
//     glDrawArrays(GL_POINTS, 0, 1);
// }
// 
// void Renderer::DrawTexture(Texture* texture, const Rect* src_rect, const Rect* dst_rect, const Color* color, float degree, FlipType flip) {
//     if (!texture)
//         return;
//     useTextureProgram();
//     float data[] = {
//         0, 0, 0, 0,
//         0, 1, 0, 1,
//         1, 0, 1, 0,
// 
//         0, 1, 0, 1,
//         1, 0, 1, 0,
//         1, 1, 1, 1
//     };
// 
//     int w = texture->GetSize().w,
//         h = texture->GetSize().h;
//     if (src_rect) {
//         Point right_top = {
//             (src_rect->x+src_rect->w)/static_cast<double>(w),
//             (h-src_rect->y)/static_cast<double>(h)
//         },
//         left_bottom = {
//             (src_rect->x)/static_cast<double>(w),
//             (h-src_rect->y-src_rect->h)/static_cast<double>(h)
//         };
// 
//         data[2] = left_bottom.x;
//         data[3] = left_bottom.y;
// 
//         data[6] = left_bottom.x;
//         data[7] = right_top.y;
// 
//         data[10] = right_top.x;
//         data[11] = left_bottom.y;
// 
//         data[14] = left_bottom.x;
//         data[15] = right_top.y;
// 
//         data[18] = right_top.x;
//         data[19] = left_bottom.y;
// 
//         data[22] = right_top.x;
//         data[23] = right_top.y;
//     }
// 
//     bindBuffers(texture->tex_);
//     glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
//     glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4*sizeof(float), 0);
//     glEnableVertexAttribArray(0);
// 
//     glm::mat4 model = glm::mat4(1.0f);
//     Rect rect;
//     if (dst_rect) {
//         rect = *dst_rect;
//     } else {
//         rect.x = 0;
//         rect.y = 0;
//         rect.w = drawable_size_.w;
//         rect.h = drawable_size_.h;
//     }
//     model = glm::translate(model, glm::vec3(rect.w/2.0+rect.x, rect.h/2+rect.y, 0));
//     model = glm::rotate(model, glm::radians(degree), glm::vec3(0, 0, 1));
//     float scale_x = rect.w,
//           scale_y = rect.h;
//     if (flip & FLIP_HORIZONTAL) {
//         scale_x = -scale_x;
//     }
//     if (flip & FLIP_VERTICAL) {
//         scale_y = -scale_y;
//     }
//     model = glm::scale(model, glm::vec3(scale_x, scale_y, 1));
//     model = glm::translate(model, glm::vec3(-0.5, -0.5, 0));
// 
//     texture_program_->UniformMat4f("model", model);
//     texture_program_->UniformMat4f("proj", current_proj_);
//     texture_program_->Uniform1i("Texture", 0);
//     if (color) {
//         texture_program_->Uniform3f("TextureColor256", color->r, color->g, color->b);
//     } else {
//         texture_program_->Uniform3f("TextureColor256", 255, 255, 255);
//     }
// 
//     if (texture->tex_) {
//         glDrawArrays(GL_TRIANGLES, 0, 6);
//         glBindTexture(GL_TEXTURE_2D, 0);
//     }
// }

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

Renderer::~Renderer() {
    destroy();
}

void Renderer::destroy() {
    delete pure_color_program_;
    // delete geo_colorful_program_;
    delete texture_program_;

    delete camera_;

    GLCall(glDeleteBuffers(1, &vbo_));
    vbo_ = 0;
    GLCall(glDeleteBuffers(1, &ebo_));
    ebo_ = 0;
    GLCall(glDeleteVertexArrays(1, &vao_));
    vao_ = 0;
}

Renderer* CreateRenderer(int window_width, int window_height) {
    Renderer* render = new Renderer(window_width, window_height);
    return render;
}

void DestroyRenderer(Renderer* render) {
    delete render;
}

};
