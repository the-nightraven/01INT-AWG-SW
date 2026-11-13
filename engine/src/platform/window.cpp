#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_video.h>
#include <awg/core/logger.hpp>
#include <awg/core/types.hpp>
#include <awg/platform/window.hpp>

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_init.h>

#include <string_view>

namespace awg {

    namespace {
        std::string_view kTag = "WNDW";
    }

    Window::~Window() {
        shutdown();
    }

    bool Window::init(const WindowConfig& config) {
        if(window_ != nullptr) {
            return true;
        }

        if(!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
            Logger::error(kTag, SDL_GetError(), -1);
            return false;
        }

        sdlInited_ = true;
        Logger::info(kTag, "SDL inited");

        SDL_WindowFlags flags = 0;
        if(config.fullscreen) {
            flags |= SDL_WINDOW_FULLSCREEN;
        }
        if(config.resizable) {
            flags |= SDL_WINDOW_RESIZABLE;
        }

        window_ = SDL_CreateWindow(config.title.c_str(), config.width, config.height, flags);
        if(window_ == nullptr) {
            Logger::error(kTag, SDL_GetError(), -1);
            shutdown();
            return false;
        }

        width_ = config.width;
        height_ = config.height;
        fullscreen_ = config.fullscreen;
        sdlInited_ = true;

        Logger::info(kTag, "Created window");

        return true;
    }

    void Window::shutdown() {
        if(window_ != nullptr) {
            SDL_DestroyWindow(window_);
            window_ = nullptr;
            Logger::info(kTag, "Destroyed window");
        }

        if(sdlInited_) {
            SDL_Quit();
            sdlInited_ = false;
        }
    }

    void Window::setFullScreen(bool state) {
        if(window_ == nullptr || fullscreen_ == state) {
            return;
        }

        if(SDL_SetWindowFullscreen(window_, state)) {
            fullscreen_ = state;
        }else {
            Logger::error(kTag, SDL_GetError(), -1);
        }
    }

    void Window::toggleFullscreen() {
        setFullScreen(!fullscreen_);
    }

}