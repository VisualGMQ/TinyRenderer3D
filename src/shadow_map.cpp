#include "tinyrenderer3d/shadow_map.hpp"

namespace tinyrenderer3d {

ShadowMap::ShadowMap(int w, int h) {
    GLCall(glGenTextures(1, &tex_));
    GLCall(glBindTexture(GL_TEXTURE_2D, tex_));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
    GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    GLCall(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor));
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, w, h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr));

    GLCall(glGenFramebuffers(1, &fbo_));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, fbo_));
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tex_, 0));
    GLCall(glDrawBuffer(GL_NONE));

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        Log("shadow map framebuffer create failed");
    }

    viewport_.w = w;
    viewport_.h = h;

    unbind();
}

void ShadowMap::UseAsShadowMap(const Rect<int>& old_viewport) {
    bind();
    GLCall(glViewport(0, 0, viewport_.w, viewport_.h));
    GLCall(glClear(GL_DEPTH_BUFFER_BIT));

    old_viewport_ = old_viewport;
}

void ShadowMap::UseAsTexture() {
    GLCall(glBindTexture(GL_TEXTURE_2D, tex_));
}

void ShadowMap::DontUse() {
    unbind();
    if (old_viewport_.has_value()) {
        GLCall(glViewport(old_viewport_->position.x, old_viewport_->position.y, old_viewport_->size.w, old_viewport_->size.h));
        old_viewport_.reset();
    }
}

void ShadowMap::bind() {
    GLCall(glBindTexture(GL_TEXTURE_2D, tex_));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, fbo_));
}

void ShadowMap::unbind() {
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

ShadowMap::~ShadowMap() {
    GLCall(glDeleteBuffers(1, &tex_));
    GLCall(glDeleteFramebuffers(1, &fbo_));

    DontUse();
}

}
