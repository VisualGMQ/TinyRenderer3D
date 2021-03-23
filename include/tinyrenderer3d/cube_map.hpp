#ifndef TINYRENDERER3D_CUBE_MAP_HPP
#define TINYRENDERER3D_CUBE_MAP_HPP

#include <string>

#include "GL/glew.h"
#include "stb_image.h"
#include "itexture.hpp"
#include "tool.hpp"
#include "types.hpp"
#include "vmath.hpp"
#include "log.hpp"

namespace tinyrenderer3d {

class CubeMap: public ITexture {
 public:
    struct CubeMapTextureDatas{
        ImageData positive_x;
        ImageData positive_y;
        ImageData negative_x;
        ImageData negative_y;
        ImageData positive_z;
        ImageData negative_z;
    };

    CubeMap();
    ~CubeMap();

    void UseAsTexture() override;
    void DontUse() override;
    void UpdateData(const CubeMapTextureDatas& datas);

 private:
    GLuint tex_ = 0;

    void setTextures(const CubeMapTextureDatas&);
    void loadAndSetTexture(const ImageData&, GLenum face);
    bool checkTexturesValid(const CubeMapTextureDatas&) const;
    void bind();
    void unbind();
};

CubeMap* CreateCubeMap(const CubeMap::CubeMapTextureDatas&);
void DestroyCubeMap(CubeMap* map);

}

#endif
