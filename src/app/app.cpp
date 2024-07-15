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

//@TODO: Android APK support     WIP
//@TODO: IOS support             WIP


#include "app.h"

//variables
SDL_Window* mainFrame = NULL;
SDL_Renderer* renderer = NULL;
int is_running = 1;

//functions

G_STATUS app_init() {
    G_STATUS status;

    init_player();

    UpdateCallback_TypeDef sysExit = {false, &is_running, end_game};
    SysEvt_TypeDef sysExit_evt = {SDL_QUIT, sysExit};
    status = register_sys_event(&sysExit_evt);

    if(status == G_STATUS_FAIL) {
        log_error(APP_TAG, "Cannot register event", G_STATUS_FAIL);
        return G_STATUS_FAIL;
    }
    log_info("APP", "registered QUIT event");

    UpdateCallback_TypeDef pl_move_r = {false, get_player_instance(), player_move_right};
    UpdateCallback_TypeDef pl_move_l = {false, get_player_instance(), player_move_left};

    KeyEvt_TypeDef pl_move_r_evt = {SDL_KEYDOWN, SDL_SCANCODE_D, pl_move_r};
    KeyEvt_TypeDef pl_move_l_evt = {SDL_KEYDOWN, SDL_SCANCODE_A, pl_move_l};

    status = register_key_event(&pl_move_l_evt);
    if(status == G_STATUS_FAIL) {
        log_error(APP_TAG, "Cannot regsiter key event", G_STATUS_FAIL);
    }
    log_info(APP_TAG, "Registered event on A");

    status = register_key_event(&pl_move_r_evt);
    if(status == G_STATUS_FAIL) {
        log_error(APP_TAG, "Cannot regsiter key event", G_STATUS_FAIL);
    }
    log_info(APP_TAG, "Registered event on D");

    // UpdateCallback_TypeDef keyExit = {false, NULL, test};
    // KeyEvt_TypeDef close = {SDL_KEYDOWN, SDL_SCANCODE_A, keyExit};
    // status = register_key_event(&close);
    // if(status == G_STATUS_FAIL) {
    //     log_error(APP_TAG, "Cannot register event", G_STATUS_FAIL);
    //     return G_STATUS_FAIL;
    // }
    // log_info("APP", "registered KEY event on: A");

    status = init_window(&mainFrame, &renderer);
    if(status == G_STATUS_FAIL) {
        log_error(APP_TAG, "Cannot init window", G_STATUS_FAIL);
        return G_STATUS_FAIL;
    }
    log_info("APP", "window initialized");
    return G_STATUS_OK;
}

G_STATUS app_loop() {
    float speed = 0;
    float w = WINDOW_WIDTH / 2 - 50;
    G_STATUS status;

    //evt_process
    log_info(APP_TAG, "Loop Started!");
    while(is_running) {
        SDL_Event e;
        status = poll_events(&e);

        if(status == G_STATUS_FAIL) {
            is_running = 0;
            log_error(APP_TAG, "cannot poll event", G_STATUS_FAIL);
            return G_STATUS_FAIL;
        }


        //update
        //w += speed;
        status = updater_run_time_delta();

        if(status == G_STATUS_FAIL) {
            is_running = 0;
            log_error(APP_TAG, "cannot update objects", G_STATUS_FAIL);
            return G_STATUS_FAIL;
        }

        //render
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        //world
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawLine(renderer, 0, 380, 640, 380);

        //player
        Player_Typedef* pl = get_player_instance();
        SDL_Rect player_rect = {pl->x, pl->y, pl->h, pl->w};
        SDL_RenderDrawRect(renderer, &player_rect);
        SDL_RenderPresent( renderer );
    }
    status = deinit_window(&mainFrame, &renderer);
    return status;
}

void end_game(void *val) {
    *((int*)(val)) = 0;
}

void test(void* val) {
    SDL_Event e;
    e.type = SDL_QUIT;
    SDL_PushEvent(&e);
}