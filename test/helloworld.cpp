#include "tr3d/renderer.hpp"
#include "tr3d/tool.hpp"

int main(int, char**) {
    atexit([](){ SDL_Quit(); });
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* window = SDL_CreateWindow("hello world",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          1024, 720,
                                          SDL_WINDOW_SHOWN|SDL_WINDOW_VULKAN);
    if (!window) {
        SDL_Log("SDL window create failed");
        return 1;
    }

    tr3d::Renderer::Init(window);

    auto vertexSource = tr3d::ReadFile("./vert.spv"),
         fragSource = tr3d::ReadFile("./frag.spv");
    tr3d::Renderer::InitPipeline(tr3d::Renderer::CreateShaderModule("VertexShader", vertexSource),
                                 tr3d::Renderer::CreateShaderModule("FragmentShader", fragSource));

    bool quit = false;
    SDL_Event event;

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }
        tr3d::Renderer::DrawFrame();
    }

    tr3d::Renderer::WaitIdle();
    tr3d::Renderer::Quit();

    SDL_DestroyWindow(window);

    return 0;
}
