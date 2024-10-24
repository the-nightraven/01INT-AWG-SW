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

//externs
RenderEngine renderer_get_engine() {
    return RENDERER_ENGINE_SDL2;
}

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
    UpdateCallback_TypeDef pl_move_stop = {false, get_player_instance(), player_stop_move};

    KeyEvt_TypeDef pl_move_r_evt = {SDL_KEYDOWN, SDL_SCANCODE_D, pl_move_r, true};
    KeyEvt_TypeDef pl_move_l_evt = {SDL_KEYDOWN, SDL_SCANCODE_A, pl_move_l, true};
    KeyEvt_TypeDef pl_move_stop_r_evt = {SDL_KEYUP, SDL_SCANCODE_D, pl_move_stop, false};
    KeyEvt_TypeDef pl_move_stop_l_evt = {SDL_KEYUP, SDL_SCANCODE_A, pl_move_stop, false};

    status = register_key_event(&pl_move_l_evt);
    if(status == G_STATUS_FAIL) {
        log_error(APP_TAG, "Cannot regsiter key event", G_STATUS_FAIL);
        return G_STATUS_FAIL;
    }
    log_info(APP_TAG, "Registered keydown event on A");

    status = register_key_event(&pl_move_stop_l_evt);
    if(status == G_STATUS_FAIL) {
        log_error(APP_TAG, "Cannot register keyup event on A", G_STATUS_FAIL);
        return G_STATUS_FAIL;
    }
    log_info(APP_TAG, "Registered keyup event on A");

    status = register_key_event(&pl_move_r_evt);
    if(status == G_STATUS_FAIL) {
        log_error(APP_TAG, "Cannot regsiter key event", G_STATUS_FAIL);
        return G_STATUS_FAIL;
    }
    log_info(APP_TAG, "Registered keydown event on D");

    status = register_key_event(&pl_move_stop_r_evt);
    if(status == G_STATUS_FAIL) {
        log_error(APP_TAG, "Cannot register keyup event on D", G_STATUS_FAIL);
        return G_STATUS_FAIL;
    }
    log_info(APP_TAG, "Registered keyup event on D");

    UpdateComponent_Typedef player_movement = {get_player_instance(), process_player_movement};
    status = register_update_components(player_movement);
    if(status == G_STATUS_FAIL) {
        log_error(APP_TAG, "Cannot register player movement cb", -1);
        return G_STATUS_FAIL;
    }
    log_info(APP_TAG, "Registered player movement callback");

    // UpdateCallback_TypeDef keyExit = {false, NULL, test};
    // KeyEvt_TypeDef close = {SDL_KEYDOWN, SDL_SCANCODE_A, keyExit};
    // status = register_key_event(&close);
    // if(status == G_STATUS_FAIL) {
    //     log_error(APP_TAG, "Cannot register event", G_STATUS_FAIL);
    //     return G_STATUS_FAIL;
    // }
    // log_info("APP", "registered KEY event on: A");

    //renderer init
    status = renderer_init();
    if(status == G_STATUS_FAIL) {
        log_error(APP_TAG, "Renderer module cannot init", -1);
        return G_STATUS_FAIL;
    }
    log_info(APP_TAG, "Renderer module inited");

    //register render components
    RendererComponent_Typedef player_render = {0, true, get_player_instance(), 1, player_render_cb, NULL};
    status = renderer_register_component(player_render);
    if(status == G_STATUS_FAIL) {
        log_error(APP_TAG, "Cannot set player render function", -1);
        return G_STATUS_FAIL;
    }
    log_info(APP_TAG, "Set player render function");

    status = init_window(&mainFrame, &renderer);
    if(status == G_STATUS_FAIL) {
        log_error(APP_TAG, "Cannot init window", G_STATUS_FAIL);
        return G_STATUS_FAIL;
    }
    log_info("APP", "window initialized");
    return G_STATUS_OK;
}

G_STATUS app_loop() {
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
        status = updater_run_time_delta();

        if(status == G_STATUS_FAIL) {
            is_running = 0;
            log_error(APP_TAG, "cannot update objects", G_STATUS_FAIL);
            return G_STATUS_FAIL;
        }

        //render
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        renderer_create_frame(&renderer);

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