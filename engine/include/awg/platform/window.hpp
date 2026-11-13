#pragma once

#include "awg/core/types.hpp"
#include <SDL3/SDL_video.h>


namespace awg {

    class Window {
        private:
            SDL_Window* window_ = nullptr;
            int width_ = 0;
            int height_ = 0;
            bool fullscreen_ = false;
            bool sdlInited_ = false;

        public:
            Window() = default;
            ~Window();

            Window(const Window&) = delete;
            Window& operator=(const Window&) = delete;

            [[nodiscard]] bool init(const WindowConfig& config);
            void shutdown();

            void setFullScreen(bool state);
            void toggleFullscreen();

            [[nodiscard]] bool isFullScreen() const noexcept { return fullscreen_; }
            [[nodiscard]] int width() const noexcept { return width_; }
            [[nodiscard]] int height() const noexcept { return height_; }
            [[nodiscard]] SDL_Window* nativeHandle() const noexcept { return window_; }
    };

}