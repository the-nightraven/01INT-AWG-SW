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

//variables
SDL_Window* mainFrame = NULL;
SDL_Renderer* renderer = NULL;
int is_running = 1;

//functions

G_STATUS app_init() {
    SysEvt_TypeDef sysExit = {SDL_QUIT, &is_running, end_game};
    register_sys_event(&sysExit);
    log_info("APP", "registered_event");
    init_window(&mainFrame, &renderer);
    return G_STATUS_OK;
}

G_STATUS app_loop() {
    float speed = 0;
    float w = WINDOW_WIDTH / 2 - 50;
    G_STATUS status;

    //evt_process
    log_info("APP", "Loop Started!");
    while(is_running) {
        SDL_Event e;
        poll_events(&e);


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
    status = deinit_window(&mainFrame, &renderer);
    return status;
}

void end_game(int *val) {
    *val = 0;
}