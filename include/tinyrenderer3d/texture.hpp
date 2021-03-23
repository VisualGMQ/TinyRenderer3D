#ifndef TEXTURE_HPP
#define TEXTURE_HPP
#include <string>

#define GLEW_STATIC
#include "GL/glew.h"
#include "vmath.hpp"
#include "log.hpp"
#include "tool.hpp"
#include "itexture.hpp"

namespace tinyrenderer3d {

enum TextureType {
    TEXTURE_TYPE_TARGET,    /** this type can be target by render*/
    TEXTURE_TYPE_STATIC,    /** this is simple type */
};

class Renderer;

class Texture final: public ITexture {
 public:
    friend class Renderer;

    Texture(TextureType type, int w, int h);
    ~Texture();
    void UseAsTexture() override;
    void UseAsTarget();
    void DontUse() override;
    Size2d<int> GetSize() const { return size_; }
    void UpdateData(const ImageData& data);

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
