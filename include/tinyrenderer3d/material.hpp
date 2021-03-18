#ifndef TINYRENDERER3D_MATERIAL_HPP
#define TINYRENDERER3D_MATERIAL_HPP

#include <vector>
#include "vmath.hpp"
#include "texture.hpp"

namespace tinyrenderer3d {

struct Material final {
    std::vector<Texture*> textures;

    Color ambient = {255, 255, 255, 255};
    Color diffuse = {255, 255, 255, 255};
    Color specular = {255, 255, 255, 255};
    float shininess = 0;
};

}

#endif
