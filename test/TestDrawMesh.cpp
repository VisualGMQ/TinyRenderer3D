#include "test_framework.hpp"
#include "tinyrenderer3d/texture.hpp"
#include "tinyrenderer3d/vmath.hpp"

using namespace tinyrenderer3d;

class DrawMesh: public TestFramework {
 public:
    DrawMesh(): TestFramework("draw mesh") {
        texture_ = LoadTexture(TEXTURE_TYPE_STATIC, "test_resources/ghost.png");
        initPlane();
        initCube();
        initTextureCube();

        GetRenderer()->AddObject(&plane_);
        GetRenderer()->AddObject(&texture_cube_);
        GetRenderer()->AddObject(&cube_);
    }

    ~DrawMesh() {
        delete texture_;
    }

    void Step() override{
        cube_.rotation.x += 2;         
        cube_.rotation.z += 3;         
    }

 private:
    Mesh plane_;
    Mesh texture_cube_;
    Mesh cube_;
    Texture* texture_;

    void initPlane() {
        const vector<Vertex> vertices = {
            {{ 0.5f, 0.0f,  0.5f},  {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
            {{ 0.5f, 0.0f, -0.5f},  {1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
            {{-0.5f, 0.0f, -0.5f},  {0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
            {{-0.5f, 0.0f,  0.5f},  {0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        };

        const vector<uint> indices = {
            0, 1, 2,
            0, 2, 3
        };

        plane_.center = {0, -1, 0};
        plane_.vertices = vertices;
        plane_.indices = indices;
        plane_.scale = {10, 1, 10};
        plane_.material.ambient = {0.6, 0.6, 0.6};
        plane_.material.diffuse = {0.6, 0.6, 0.6};
        plane_.material.specular = {0.6, 0.6, 0.6};
        plane_.material.shininess = 32;
        plane_.material.diffuse_texture = texture_;
    }

    void initCube() {
        vector<Vertex> vertices = {
            {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},
            {{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}},
            {{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},
            {{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}},
            {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},
            {{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}, {0.0f, 0.0f, -1.0f}},

            {{-0.5f, -0.5f,  0.5f},  {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
            {{ 0.5f, -0.5f,  0.5f},  {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}}, 
            {{ 0.5f,  0.5f,  0.5f},  {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}}, 
            {{ 0.5f,  0.5f,  0.5f},  {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}}, 
            {{-0.5f,  0.5f,  0.5f},  {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
            {{-0.5f, -0.5f,  0.5f},  {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},

            {{-0.5f,  0.5f,  0.5f},  {1.0f, 0.0f}, {-1.0f, 0.0, 0.0f}},
            {{-0.5f,  0.5f, -0.5f},  {1.0f, 1.0f}, {-1.0f, 0.0, 0.0f}},
            {{-0.5f, -0.5f, -0.5f},  {0.0f, 1.0f}, {-1.0f, 0.0, 0.0f}},
            {{-0.5f, -0.5f, -0.5f},  {0.0f, 1.0f}, {-1.0f, 0.0, 0.0f}},
            {{-0.5f, -0.5f,  0.5f},  {0.0f, 0.0f}, {-1.0f, 0.0, 0.0f}},
            {{-0.5f,  0.5f,  0.5f},  {1.0f, 0.0f}, {-1.0f, 0.0, 0.0f}},

            {{ 0.5f,  0.5f,  0.5f},  {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}}, 
            {{ 0.5f, -0.5f, -0.5f},  {0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}}, 
            {{ 0.5f,  0.5f, -0.5f},  {1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}}, 
            {{ 0.5f, -0.5f, -0.5f},  {0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}}, 
            {{ 0.5f,  0.5f,  0.5f},  {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}}, 
            {{ 0.5f, -0.5f,  0.5f},  {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}}, 

            {{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}, {0.0f, -1.0f, 0.0f}},
            {{ 0.5f, -0.5f, -0.5f}, {1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}}, 
            {{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}}, 
            {{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}}, 
            {{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
            {{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}, {0.0f, -1.0f, 0.0f}},

            {{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
            {{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}}, 
            {{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}}, 
            {{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}}, 
            {{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
            {{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}}
        };

        cube_.center = {0, 0, 0};
        cube_.vertices = vertices;
        cube_.scale = {1, 1, 1};

        cube_.material.ambient = {0, 0.7, 0.7};
        cube_.material.diffuse = {0, 0.7, 0.7};
        cube_.material.specular = {0, 0.7, 0.7};
        cube_.material.shininess = 32;
    }

    void initTextureCube() {
        texture_cube_.vertices = cube_.vertices;
        texture_cube_.center = {1.5, 0, 0};
        texture_cube_.scale = {1, 1, 1};

        texture_cube_.material.ambient = {0, 0.8, 0.8};
        texture_cube_.material.diffuse = {0, 0.8, 0.8};
        texture_cube_.material.specular = {0, 0.8, 0.8};
        texture_cube_.material.shininess = 32;
        texture_cube_.material.diffuse_texture = texture_;
    }
};

RUN_TEST(DrawMesh)
