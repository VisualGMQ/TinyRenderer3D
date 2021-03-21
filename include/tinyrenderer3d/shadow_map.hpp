#ifndef TINYRENDERER3D_SHADPW_MAP_HPP
#define TINYRENDERER3D_SHADPW_MAP_HPP

#include <optional>

#include "GL/glew.h"
#include "vmath.hpp"
#include "tool.hpp"
#include "log.hpp"

namespace tinyrenderer3d {

class ShadowMap {
 public:
    ShadowMap(int w, int h);
    void Use(const Rect<int>& old_viewport);
    void UseAsTarget();
    void DontUse();
    ~ShadowMap();

 private:
    GLuint tex_ = 0;
    GLuint fbo_ = 0;

    Size2d<int> viewport_;
    std::optional<Rect<int>> old_viewport_;

    void bind();
    void unbind();
};

}
#endif
