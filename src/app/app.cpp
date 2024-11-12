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

G_STATUS app_init() {
    G_STATUS status;

    status = monitor_init();
    if(status == G_STATUS_FAIL) {
        log_error(APP_TAG, "Cannot init engine monitor module", -1);
        return G_STATUS_FAIL;
    }
    log_info(APP_TAG, "Inited engine monitor module");

    status = monitor_init_modules();
    if(status == G_STATUS_FAIL) {
        log_error(APP_TAG, "Cannot init engine modules", -1);
        return G_STATUS_FAIL;
    }
    log_info(APP_TAG, "Inited engine modules");

    status = init_player();
    if(status == G_STATUS_FAIL) {
        log_error(APP_TAG, "Cannot init player", -1);
        return G_STATUS_FAIL;
    }
    log_info(APP_TAG, "Inited player");

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


    //register render components
    RendererComponent_Typedef player_render = {0, true, get_player_instance(), 1, player_render_cb, NULL};
    status = renderer_register_component(player_render);
    if(status == G_STATUS_FAIL) {
        log_error(APP_TAG, "Cannot set player render function", -1);
        return G_STATUS_FAIL;
    }
    log_info(APP_TAG, "Set player render function");

    status = monitor_init_window_module();
    if(status == G_STATUS_FAIL) {
        log_error(APP_TAG, "Cannot init engine display module", -1);
        return G_STATUS_FAIL;
    }
    log_info(APP_TAG, "Inited engine display module");

    status = monitor_check_env();
    if(status == G_STATUS_FAIL) {
        log_error(APP_TAG, "Engine modules not inited successfuly", -1);
        return G_STATUS_FAIL;
    }
    log_info(APP_TAG, "Engine modules inited successfully");

    status = monitor_start_updating();
    if(status == G_STATUS_FAIL) {
        log_error(APP_TAG, "Rendering did not start", -1);
        return G_STATUS_FAIL;
    }
    log_info(APP_TAG, "Renderer forked successfuly");

    return G_STATUS_OK;
}

G_STATUS app_loop() {
    G_STATUS status;
    SDL_Renderer* eng_renderer = monitor_get_renderer_instance();
    SDL_Window* eng_display = monitor_get_display_instance();

    log_info(APP_TAG, "Loop Started!");
    while(monitor_get_run_cond()) {
        status = monitor_process_loop();
        if(status == G_STATUS_FAIL) {
            log_error(APP_TAG, "Monitor registered errors", -1);
            monitor_force_exit();
            return G_STATUS_FAIL;
        }
    }
    return status;
}

G_STATUS app_deinit() {
    monitor_deinit_modules();
    return G_STATUS_OK;
}