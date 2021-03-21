#ifndef TINYRENDERER3D_MODEL_HPP
#define TINYRENDERER3D_MODEL_HPP

#include <list>
#include "mesh.hpp"
#include "tinyrenderer3d/drawable.hpp"

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

namespace tinyrenderer3d {

using std::list;

class Model: public Drawable {
 public:
    Model() = default;
    Model(const string& obj_filename);
    void Load(const string& obj_filename);

    void SetPosition(float x, float y, float z);
    Vec3<float> GetPosition() const { return position_; }

    void SetScale(float x, float y, float z);
    Vec3<float> GetScale() const { return scale_; }

    void SetRotation(float x, float y, float z);
    Vec3<float> GetRotation() const { return rotation_; }

    void Draw(Program*) override;

 private:
    list<Mesh> meshes_;
    string directory_;

    Vec3<float> position_ = {0, 0, 0};
    Vec3<float> scale_ = {1, 1, 1};
    Vec3<float> rotation_ = {0, 0, 0};

    vector<Texture*> loadMaterialTextures(aiMaterial* material, aiTextureType type, string typeName);
};

}

#endif
