#ifndef TINYRENDERER3D_ITEXTURE_HPP
#define TINYRENDERER3D_ITEXTURE_HPP

#include <string>

#include "types.hpp"
#include "log.hpp"
#include "vmath.hpp"
#include "stb_image.h"

namespace tinyrenderer3d {

class ITexture {
 public:
    virtual ~ITexture() = default;

    virtual void UseAsTexture() = 0;
    virtual void DontUse() = 0;
};

enum PixelFormat {
    PIXEL_FORMAT_UNKNOWN,
    PIXEL_FORMAT_RGB888,
    PIXEL_FORMAT_RGBA8888
};

struct ImageData {
    uchar* data = nullptr;
    Size2d<uint> size = {0, 0};
    PixelFormat format = PIXEL_FORMAT_UNKNOWN;
};

ImageData LoadImage(const std::string& filename);
void DestroyImage(ImageData& data);

}

#endif
