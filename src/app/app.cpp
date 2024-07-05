/*
###########################################################

@file:app.cpp

@author: Tanasescu Darius

@Licence: Apache2.0

@Copyleft the-nightraven, product is open source and has
absolutely no warranty, you are free to copy, distribute
and change, but not for commercial use

@Revision: InfDev

@Description: app logic

###########################################################
*/

#include "app.h"

SDL_Window* mainFrame = NULL;
SDL_Renderer* renderer = NULL;

G_STATUS app_init() {
    init_window(&mainFrame, &renderer);
    return G_STATUS_OK;
}

G_STATUS app_loop() {
    bool quit = false;
    float speed = 0;
    float w = WINDOW_WIDTH / 2 - 50;
    G_STATUS status;

    //evt_process
    while(!quit) {
        SDL_Event e;
        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_QUIT) {
                quit = true;
            }
            if(e.type == SDL_KEYUP) {
                speed = 0;
            }else if(e.type == SDL_KEYDOWN) {
                if(e.key.keysym.scancode == SDL_SCANCODE_LEFT) {
                    speed = -0.1;
                }else if(e.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
                    speed = 0.1;
                }
            }
        }

        //update
        w += speed;


        //render
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawLine(renderer, 0, 380, 640, 380);

        SDL_Rect player = {w, 330, 50, 50};
        SDL_RenderDrawRect(renderer, &player);
        SDL_RenderPresent( renderer );
    }
    status = deinit_window(&mainFrame);
    return status;
}