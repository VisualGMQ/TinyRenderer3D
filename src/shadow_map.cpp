#include "tinyrenderer3d/shadow_map.hpp"

namespace tinyrenderer3d {

ShadowMap::ShadowMap(int w, int h) {
    GLCall(glGenTextures(1, &tex_));
    GLCall(glBindTexture(GL_TEXTURE_2D, tex_));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, w, h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr));

    GLCall(glGenFramebuffers(1, &fbo_));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, fbo_));
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tex_, 0));
    GLCall(glDrawBuffer(GL_NONE));
    GLCall(glReadBuffer(GL_NONE));

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        Log("shadow map framebuffer create failed");
    }

    DontUse();
}

void ShadowMap::Use() {
    GLCall(glBindTexture(GL_TEXTURE_2D, tex_));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, fbo_));
}

void ShadowMap::DontUse() {
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

ShadowMap::~ShadowMap() {
    GLCall(glDeleteBuffers(1, &tex_));
    GLCall(glDeleteFramebuffers(1, &fbo_));

    DontUse();
}

}
