#ifndef TINYRENDERER3D_SHADPW_MAP_HPP
#define TINYRENDERER3D_SHADPW_MAP_HPP

#include "GL/glew.h"
#include "tool.hpp"
#include "log.hpp"

namespace tinyrenderer3d {

class ShadowMap {
 public:
    ShadowMap(int w, int h);
    void Use();
    void DontUse();
    ~ShadowMap();

 private:
    GLuint tex_ = 0;
    GLuint fbo_ = 0;
};

}
#endif
