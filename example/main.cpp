/*
###########################################################

@file: main.cpp

@author: Tanasescu Darius

@Licence: Apache2.0

@Copyleft the-nightraven, product is open source and has
absolutely no warranty, you are free to copy, distribute
and change, but not for commercial use

@Revision: InfDev

@Description: Main entrypoint in application

###########################################################
*/

#include "awg/core/types.hpp"
#include "awg/platform/window.hpp"
#include <awg/core/logger.hpp>

#include <iostream>

#if defined(_WIN32) && !defined(DEBUG)
    #define SDL_MAIN_HANDLED
    #include <SDL3/SDL_main.h>
    #include <windows.h>
#endif

namespace {

    constexpr const char* kTag = "MAIN";

    int runApp() {
        awg::Window window = awg::Window();
        if(window.init(awg::WindowConfig{})) {
            awg::Logger::info(kTag, "Window inited");
        }else {
            awg::Logger::error(kTag, "Cannot init", -1);
            return -1;
        }

        char a;
        std::cin >> a;

        return 0;
    }

}



#if defined(_WIN32) && !defined(DEBUG)

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    (void)hInstance;
    (void)hPrevInstance;
    (void)lpCmdLine;
    (void)nCmdShow;

    SDL_SetMainReady();
    return runApp();
}

#else

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    return runApp();
}

#endif