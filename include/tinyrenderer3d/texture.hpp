#ifndef TEXTURE_HPP
#define TEXTURE_HPP
#include <string>

#define GLEW_STATIC
#include "GL/glew.h"
#include "vmath.hpp"
#include "log.hpp"
#include "tool.hpp"

namespace tinyrenderer3d {

enum TextureType {
    TEXTURE_TYPE_TARGET,    /** this type can be target by render*/
    TEXTURE_TYPE_STATIC     /** this is simple type */
};

enum PixelFormat {
    PIXEL_FORMAT_RGB888,
    PIXEL_FORMAT_RGBA8888
};

class Renderer;

class Texture final {
 public:
    friend class Renderer;

    Texture(TextureType type, int w, int h);
    ~Texture();
    void UseAsTexture();
    void UseAsTarget();
    void DontUse();
    Size2d<int> GetSize() const { return size_; }
    void UpdateData(unsigned char* data, int w, int h, PixelFormat format);

 private:
    Size2d<int> size_;

    GLuint tex_ = 0;
    GLuint fbo_ = 0;
};

Texture* LoadTexture(TextureType type, std::string filename);
Texture* CreateTexture(TextureType type, int w, int h);
void DestroyTexture(Texture* texture);

};

#endif
