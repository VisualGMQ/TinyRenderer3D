#ifndef APP_HPP
#define APP_HPP

#include "SDL_events.h"
#include "glm/geometric.hpp"
#include "tinyrenderer3d/tinyrenderer3d.hpp"

#include "SDL.h"
#define GLEW_STATIC
#include "GL/glew.h"

constexpr int WindowWidth = 800;
constexpr int WindowHeight = 600;

class App {
 public:
    App() {
        initSDL();
        createRender();
    }

    void MainLoop() {
        while (!should_quit_) {
            render_->SetClearColor(0, 0, 0);
            render_->Clear();
            while (SDL_PollEvent(&event_)) {
                if (event_.type == SDL_QUIT)
                    Exit();
                if (event_.type == SDL_KEYDOWN) {
                    if (event_.key.keysym.sym == SDLK_s) {
                        camera_->MoveByMyself({0, 0, 0.2});
                    }
                    if (event_.key.keysym.sym == SDLK_w) {
                        camera_->MoveByMyself({0, 0, -0.2});
                    }
                    if (event_.key.keysym.sym == SDLK_a) {
                        camera_->MoveByMyself({-0.2, 0, 0});
                    }
                    if (event_.key.keysym.sym == SDLK_d) {
                        camera_->MoveByMyself({0.2, 0, 0});
                    }
                }
                if (event_.type == SDL_MOUSEMOTION) {
                    camera_->RotateBy(event_.motion.yrel*0.3, event_.motion.xrel*0.3, 0);
                }
            }
            step();
            SDL_GL_SwapWindow(window_);
            SDL_Delay(30);
        }
    }

    void Exit() {
        should_quit_ = true;
    }

    virtual ~App() {
        destroyRender();
        quitSDL();
    }

 protected:
    SDL_Window* window_ = nullptr;
    tinyrenderer3d::Renderer* render_;
    tinyrenderer3d::Camera* camera_;

 private:
    SDL_Event event_;
    bool should_quit_ = false;

    void initSDL() {
        SDL_Init(SDL_INIT_EVERYTHING);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 2);
        window_ = SDL_CreateWindow(
                "hello world",
                SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                WindowWidth, WindowHeight,
                SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL);
        if (!window_)
            throw std::runtime_error("window create failed");
        SDL_GL_CreateContext(window_);
        glewInit();
    }

    virtual void step() {

    }

    void createRender() {
        render_ = tinyrenderer3d::CreateRenderer(WindowWidth, WindowHeight);
        camera_ = render_->GetCamera();
    }

    void destroyRender() {
        tinyrenderer3d::DestroyRenderer(render_);
    }

    void quitSDL() {
        SDL_Quit();
    }
};

#endif
