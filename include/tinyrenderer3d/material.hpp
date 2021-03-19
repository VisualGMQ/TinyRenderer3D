#ifndef TINYRENDERER3D_MATERIAL_HPP
#define TINYRENDERER3D_MATERIAL_HPP

#include <vector>
#include "vmath.hpp"
#include "texture.hpp"
#include "program.hpp"

namespace tinyrenderer3d {

struct Material final {
    std::vector<Texture*> textures;

    Color ambient = {255, 255, 255, 255};
    Color diffuse = {255, 255, 255, 255};
    Color specular = {255, 255, 255, 255};
    float shininess = 0;

    bool HasTexture() const { return !textures.empty(); }
    void Use(Program* program);
    void DontUse();
};

}

#endif
