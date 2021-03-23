#include "tinyrenderer3d/texture.hpp"
#include "tinyrenderer3d/itexture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace tinyrenderer3d {

Texture::Texture(TextureType type, int w, int h) {
    if (w > 0 && h > 0) {
        GLCall(glGenTextures(1, &tex_));
        GLCall(glBindTexture(GL_TEXTURE_2D, tex_));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL));
        if (type == TEXTURE_TYPE_TARGET) {
            GLCall(glGenFramebuffers(1, &fbo_));
            GLCall(glBindFramebuffer(GL_FRAMEBUFFER, fbo_));
            GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex_, 0));
            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
                Log("framebuffer create incomplete");
            }
            GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
        }
        size_.w = w;
        size_.h = h;
    }
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void Texture::UseAsTexture() {
    GLCall(glBindTexture(GL_TEXTURE_2D, tex_));
}

void Texture::UseAsTarget() {
    GLCall(glBindTexture(GL_TEXTURE_2D, tex_));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, fbo_));
}

void Texture::DontUse() {
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void Texture::UpdateData(const ImageData& data) {
    GLCall(glBindTexture(GL_TEXTURE_2D, tex_));
    GLsizei w = data.size.w, h = data.size.h;
    if (data.format == PIXEL_FORMAT_RGB888) {
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data.data));
    } else if (data.format == PIXEL_FORMAT_RGBA8888) {
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data));
    } else {
        Log("Unknown image pixel format");
    }
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

Texture::~Texture() {
    GLCall(glDeleteTextures(1, &tex_));
    GLCall(glDeleteFramebuffers(1, &fbo_));
}

Texture* CreateTexture(TextureType type, int w, int h) {
    Texture* texture = new Texture(type, w, h);
    return texture;
}

Texture* LoadTexture(TextureType type, std::string filename) {
    ImageData data = LoadImage(filename);
    Texture* texture = CreateTexture(type, data.size.w, data.size.h);
    texture->UpdateData(data);
    DestroyImage(data);
    return texture;
}

void DestroyTexture(Texture* texture) {
    delete texture;
}

};
