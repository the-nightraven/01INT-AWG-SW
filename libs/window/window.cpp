/*
###########################################################

@file:window.cpp

@author: Tanasescu Darius

@Licence: Apache2.0

@Copyleft the-nightraven, product is open source and has
absolutely no warranty, you are free to copy, distribute
and change, but not for commercial use

@Revision: InfDev

@Description: window handler

###########################################################
*/

#include "window.h"
#include "logger.h"


G_STATUS init_window(SDL_Window **mainFrame, SDL_Renderer **renderer) {
    *mainFrame = nullptr;
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        log_error("SDL", "Could not init video, code: ", G_STATUS_FAIL);
        return G_STATUS_FAIL;
    }
    log_info(WINDOW_TAG, "SDL Inited");

    *mainFrame = SDL_CreateWindow("AWG", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

    if(*mainFrame == nullptr) {
        log_error(WINDOW_TAG, "Could not create window", G_STATUS_FAIL);
        return G_STATUS_FAIL;
    }
    log_info(WINDOW_TAG, "Created window");
    *renderer = SDL_CreateRenderer(*mainFrame, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if(*renderer == nullptr) {
        log_error(WINDOW_TAG, "Could not init renderer", G_STATUS_FAIL);
        return G_STATUS_FAIL;
    }

    SDL_SetRenderDrawBlendMode(*renderer, SDL_BLENDMODE_BLEND);

#if DEBUG
    SDL_RendererInfo info;
    SDL_GetRendererInfo(*renderer, &info);
    printf("%s Using renderer: %s\n", WINDOW_TAG, info.name);
#endif

    return G_STATUS_OK;
}

G_STATUS deinit_window(SDL_Window **mainFrame, SDL_Renderer **renderer) {
    if(*mainFrame == nullptr) {
        log_error(WINDOW_TAG, "Screen doesnt exist", G_STATUS_FAIL);
        return G_STATUS_FAIL;
    }

    if(*renderer == nullptr) {
        log_error(WINDOW_TAG, "Renderer doesnt exist", G_STATUS_FAIL);
        return G_STATUS_FAIL;
    }

    SDL_DestroyRenderer(*renderer);
    SDL_DestroyWindow(*mainFrame);
    log_info(WINDOW_TAG, "Destroyed window");
    SDL_Quit();

    return G_STATUS_OK;
}

// SDL_Window* get_window_instance() {
//     return 
// }

// SDL_Renderer* get_renderer_instance() {

// }
