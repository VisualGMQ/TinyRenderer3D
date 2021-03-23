#ifndef TINYRENDERER3D_MATERIAL_HPP
#define TINYRENDERER3D_MATERIAL_HPP

#include <vector>
#include "vmath.hpp"
#include "texture.hpp"
#include "program.hpp"

namespace tinyrenderer3d {

using std::vector;

struct Material final {
    Color3<float> ambient = {0, 0, 0};
    Color3<float> diffuse = {0, 0, 0};
    Color3<float> specular = {0, 0, 0};
    float shininess = 0;
    // NOTE: temporary only support one texture
    // vector<Texture*> ambient_textures;
    // vector<Texture*> diffuse_textures;
    // vector<Texture*> specular_textures;
    // TODO currently we don't support normal texture
    ITexture* normal_texture = nullptr;
    ITexture* diffuse_texture = nullptr;
    ITexture* specular_texture = nullptr;
    ITexture* emission_texture = nullptr;

    void Use(Program* program);
    void DontUse();
};

}

#endif
