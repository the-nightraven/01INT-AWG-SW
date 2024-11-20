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

    status = player_register_events();
    if(status == G_STATUS_FAIL) {
        log_error(APP_TAG, "Cannot init player events", -1);
        return G_STATUS_FAIL;
    }
    log_info(APP_TAG, "Inited player events");

    //register render components
    RendererComponent_Typedef player_render = {0, "Player", true, get_player_instance(), 1, player_render_cb, nullptr};
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