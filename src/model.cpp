#include "tinyrenderer3d/model.hpp"
#include "assimp/material.h"
#include "assimp/postprocess.h"
#include "tinyrenderer3d/vmath.hpp"

namespace tinyrenderer3d {


Model::Model(const string& obj_filename) {
    Load(obj_filename);
}

void Model::Load(const string& obj_filename) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(obj_filename, aiProcess_Triangulate|aiProcess_FixInfacingNormals|aiProcess_FlipUVs|aiProcess_GenSmoothNormals);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        Log("assimp error: %s", importer.GetErrorString());
        return;
    }
    directory_ = obj_filename.substr(0, obj_filename.find_last_of('/')) + "/";

    for (int i = 0; i < scene->mNumMeshes; i++) {
        aiMesh* aimesh = scene->mMeshes[i];
        meshes_.emplace_back();
        Mesh& mesh = meshes_.back();
        Vertex vertex;
        for (int v = 0; v < aimesh->mNumVertices; v++) {
            aiVector3D position = aimesh->mVertices[v];
            vertex.position.x = position.x;
            vertex.position.y = position.y;
            vertex.position.z = position.z;

            aiVector3D normal = aimesh->mNormals[v];
            vertex.normal.x = normal.x;
            vertex.normal.y = normal.y;
            vertex.normal.z = normal.z;

            if (aimesh->mTextureCoords[0]) {
                vertex.texcoord.x = aimesh->mTextureCoords[0][i].x;
                vertex.texcoord.y = aimesh->mTextureCoords[0][i].y;
            } else {
                vertex.texcoord = {0, 0};
            }
            mesh.vertices.push_back(vertex);
        }
        for (int j = 0; j < aimesh->mNumFaces; j++) {
            aiFace face = aimesh->mFaces[j];
            for(uint index = 0; index < face.mNumIndices; index++) {
                mesh.indices.push_back(face.mIndices[index]);
            }
        }
        Material material;
        if (aimesh->mMaterialIndex >= 0) {
            aiMaterial* aimaterial = scene->mMaterials[aimesh->mMaterialIndex];
            aiColor3D color;

            aimaterial->Get(AI_MATKEY_COLOR_AMBIENT, color);
            material.ambient.r = color.r;
            material.ambient.g = color.g;
            material.ambient.b = color.b;

            aimaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);
            material.diffuse.r = color.r;
            material.diffuse.g = color.g;
            material.diffuse.b = color.b;

            aimaterial->Get(AI_MATKEY_COLOR_SPECULAR, color);
            material.specular.r = color.r;
            material.specular.g = color.g;
            material.specular.b = color.b;

            aimaterial->Get(AI_MATKEY_SHININESS, mesh.material.shininess);

            vector<Texture*> diffuse_textures = loadMaterialTextures(aimaterial, aiTextureType_DIFFUSE, "texture_diffuse");
            if (!diffuse_textures.empty()) {
                material.diffuse_texture = diffuse_textures[0]; //  FIXME to suuport multi texture
            }
            vector<Texture*> specular_textures = loadMaterialTextures(aimaterial, aiTextureType_SPECULAR, "texture_specular");
            if (!specular_textures.empty()) {
                material.specular_texture = specular_textures[0]; //  FIXME to suuport multi texture
            }
        }
        mesh.material = material;
    }
}

vector<Texture*> Model::loadMaterialTextures(aiMaterial* material, aiTextureType type, string typeName) {
    vector<Texture*> textures;
    for(unsigned int i = 0; i < material->GetTextureCount(type); i++) {
        aiString str;
        material->GetTexture(type, i, &str);
        Texture* texture = LoadTexture(TEXTURE_TYPE_STATIC, directory_ + string(str.C_Str()));
        textures.push_back(texture);
    }
    return textures;
}

void Model::SetPosition(float x, float y, float z) {
    position_.x = x;
    position_.y = y;
    position_.z = z;
    for (Mesh& mesh: meshes_) {
        mesh.center = position_;
    }
}

void Model::SetScale(float x, float y, float z) {
    scale_.x = x;
    scale_.y = y;
    scale_.z = z;
    for (Mesh& mesh: meshes_) {
        mesh.scale = scale_;
    }
}

void Model::SetRotation(float x, float y, float z) {
    rotation_.x = x;
    rotation_.y = y;
    rotation_.z = z;
    for (Mesh& mesh: meshes_) {
        mesh.rotation = rotation_;
    }
}

void Model::Draw(Program* program) {
    for (Mesh& mesh: meshes_) {
        mesh.Draw(program);
    }
}

}

