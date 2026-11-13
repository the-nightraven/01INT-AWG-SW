#pragma once

#include <cstdint>
#include <string>


namespace awg {

    enum class Status : int {
        Ok = 0,
        Fail = 1
    };

    struct Vec2 {
        float x = 0.0f;
        float y = 0.0f;
    };

    struct Rect {
        float x = 0.0f;
        float y = 0.0f;
        float w = 0.0f;
        float h = 0.0f;

        [[nodiscard]] constexpr bool contains(float px, float py) const noexcept {
            return px > x && px < (x + w) && py > y && py < (y + h);
        }
    };

    struct Color {
        std::uint8_t r = 0;
        std::uint8_t g = 0;
        std::uint8_t b = 0;
        std::uint8_t a = 255;

        static constexpr Color black() noexcept { return {0,0,0,255}; }
        static constexpr Color white() noexcept { return {255,255,255,255}; }
        static constexpr Color transparent() noexcept { return {0,0,0,0}; }
    };

    struct WindowConfig {
        std::string title = "AWG";
        int width = 1280;
        int height = 720;
        bool fullscreen = false;
        bool resizable = false;
    };

}