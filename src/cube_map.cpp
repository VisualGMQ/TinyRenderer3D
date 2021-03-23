#include "tinyrenderer3d/cube_map.hpp"
#include "tinyrenderer3d/itexture.hpp"

namespace tinyrenderer3d {

CubeMap::CubeMap() {
    GLCall(glGenTextures(1, &tex_));
    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, tex_));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
    unbind();
}

void CubeMap::UpdateData(const CubeMapTextureDatas& datas) {
    if (!checkTexturesValid(datas)) {
        Log("please ensure your image's size are the same as each other");
        return;
    }
    bind();
    loadAndSetTexture(datas.negative_x, GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
    loadAndSetTexture(datas.negative_y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
    loadAndSetTexture(datas.negative_z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);
    loadAndSetTexture(datas.positive_x, GL_TEXTURE_CUBE_MAP_POSITIVE_X);
    loadAndSetTexture(datas.positive_y, GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
    loadAndSetTexture(datas.positive_z, GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
    unbind();
}

// a small function to check sizes area equal to each other
bool checkEqual(const Size2d<uint>* sizes, int idx, int len) {
    if (idx < len-1) {
        if (sizes[idx] == sizes[idx+1]) {
            return checkEqual(sizes, idx+1, len);
        } else {
            return false;
        }
    }
    return true;
}

bool CubeMap::checkTexturesValid(const CubeMapTextureDatas& datas) const {
    Size2d<uint> sizes[] = {
        datas.negative_x.size,
        datas.negative_y.size,
        datas.negative_z.size,
        datas.positive_x.size,
        datas.positive_y.size,
        datas.negative_z.size
    };
    return checkEqual(sizes, 0, 6);
}

void CubeMap::loadAndSetTexture(const ImageData& image_data, GLenum face) {
    GLsizei w = image_data.size.w, h = image_data.size.h;
    if (image_data.format == PIXEL_FORMAT_RGBA8888) {
        glTexImage2D(face, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data.data);
    } else {
        glTexImage2D(face, 0, GL_RGBA, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data.data);
    }
}

CubeMap::~CubeMap() {
    GLCall(glDeleteTextures(1, &tex_));
}

void CubeMap::UseAsTexture() {
    bind();
}

void CubeMap::DontUse() {
    unbind();
}

void CubeMap::bind() {
    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, tex_));
}

void CubeMap::unbind() {
    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
}

CubeMap* CreateCubeMap(const CubeMap::CubeMapTextureDatas& data) {
    auto* cubemap = new CubeMap;
    cubemap->UpdateData(data);
    return cubemap;
}

void DestroyCubeMap(CubeMap* map) {
    delete map;
}

}
