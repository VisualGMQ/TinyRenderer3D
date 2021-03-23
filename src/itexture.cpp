#include "tinyrenderer3d/itexture.hpp"

tinyrenderer3d::ImageData tinyrenderer3d::LoadImage(const std::string& filename) {
    ImageData data;
    int w, h, channels;
    data.data = stbi_load(filename.c_str(), &w, &h, &channels, 0);
    if (!data.data) {
        Log("%s load failed", filename.c_str());
        return data;
    }
    data.size.w = w;
    data.size.h = h;
    if (channels == 4) {
        data.format = PIXEL_FORMAT_RGBA8888;
    } else if (channels == 3){
        data.format = PIXEL_FORMAT_RGB888;
    } else {
        Log("image pixel format unknown");
        data.format = PIXEL_FORMAT_UNKNOWN;
    }
    return data;
}

void tinyrenderer3d::DestroyImage(ImageData& data) {
    stbi_image_free(data.data);
    data.size = {0, 0};
    data.format = PIXEL_FORMAT_UNKNOWN;
}
