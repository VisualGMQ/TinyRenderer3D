#ifndef TEST_TEST_FRAMEWORK_HPP
#define TEST_TEST_FRAMEWORK_HPP

#include <string>
#include <map>

#include "SDL_events.h"
#include "SDL_timer.h"
#include "SDL_video.h"
#include "tinyrenderer3d/renderer.hpp"
#include "SDL.h"
using namespace tinyrenderer3d;

using std::string;
using std::map;

constexpr int WindowWidth = 800;
constexpr int WindowHeight = 600;

class TestFramework {
 public:
    TestFramework(const string& title) {
        SDL_Init(SDL_INIT_EVERYTHING);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 2);

        window_ = SDL_CreateWindow(title.c_str(),
                                   SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                   WindowWidth, WindowHeight,
                                   SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN);

        SDL_GL_CreateContext(window_);
        glewInit();

        render_ = CreateRenderer(WindowWidth, WindowHeight);
    }

    virtual ~TestFramework() {
        DestroyRenderer(render_);
        SDL_Quit();
    }

    void MainLoop() {
        while (!isquit_) {
            render_->SetClearColor(0, 0, 0);
            render_->Clear();
            while (SDL_PollEvent(&event_)) {
                if (event_.type == SDL_QUIT) {
                    isquit_ = true;
                }
                if (event_.type == SDL_MOUSEBUTTONDOWN) {
                    mousebutton_down_ = true;
                }
                if (event_.type == SDL_MOUSEBUTTONUP) {
                    mousebutton_down_ = false;
                }
                if (event_.type == SDL_KEYDOWN) {
                    keys_[event_.key.keysym.sym] = true;
                }
                if (event_.type == SDL_KEYUP) {
                    keys_[event_.key.keysym.sym] = false;
                }
                if (event_.type == SDL_MOUSEMOTION) {
                    if (mousebutton_down_) {
                        GetRenderer()->GetCamera()->RotateBy(event_.motion.xrel*0.2);
                    }
                }
            }
            cameraUpdate();
            Step();
            render_->Draw();
            SDL_GL_SwapWindow(window_);
            SDL_Delay(30);
        }
    }

    bool IsMouseButtonDown() const { return mousebutton_down_; }

    bool IsKeyPress(SDL_Keycode key) {
        if (keys_.find(key) != keys_.end() && keys_[key])
            return true;
        return false;
    }

    Renderer* GetRenderer() {
        return render_;
    }

    virtual void Step() {}

 private:
    SDL_Event event_;
    SDL_Window* window_;
    bool isquit_ = false;

    map<SDL_Keycode, bool> keys_;
    bool mousebutton_down_ = false;

    Renderer* render_;

    void cameraUpdate() {
        Camera* camera = GetRenderer()->GetCamera();
        if (IsKeyPress(SDLK_a)) {
            camera->MoveByMyself({-0.2, 0, 0});
        } else if (IsKeyPress(SDLK_d)) {
            camera->MoveByMyself({0.2, 0, 0});
        } else if (IsKeyPress(SDLK_w)) {
            camera->MoveByMyself({0, 0, -0.2});
        } else if (IsKeyPress(SDLK_s)) {
            camera->MoveByMyself({0, 0, 0.2});
        }
    }
};

#define RUN_TEST(classname) \
int main(int argc, char** argv) { \
    classname app; \
    app.MainLoop(); \
    return 0; \
}

#endif
