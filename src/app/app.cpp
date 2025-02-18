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
#include "game_scenes/menu/start_menu/start_menu.h"

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

    status = monitor_check_env();
    if(status == G_STATUS_FAIL) {
        log_error(APP_TAG, "Engine modules not inited successfuly", -1);
        return G_STATUS_FAIL;
    }
    log_info(APP_TAG, "Engine modules inited successfully");

    status = monitor_start_updating();
    if(status == G_STATUS_FAIL) {
        log_error(APP_TAG, "updating did not start", -1);
        return G_STATUS_FAIL;
    }
    log_info(APP_TAG, "updater forked successfuly");


    //start menu test
    status = init_start_menu();
    if(status == G_STATUS_FAIL) {
        log_error(APP_TAG, "Cannot init start menu", -1);
        return G_STATUS_FAIL;
    }
    log_info(APP_TAG, "Inited start menu");

    status = register_start_menu();
    if(status == G_STATUS_FAIL) {
        log_error(APP_TAG, "Cannot register start menu", -1);
        return G_STATUS_FAIL;
    }
    log_info(APP_TAG, "Registered start menu");

    status = init_player();
    if(status == G_STATUS_FAIL) {
        log_error(APP_TAG, "Cannot init player", -1);
        return G_STATUS_FAIL;
    }
    log_info(APP_TAG, "Inited player");
    status = player_register_events();
    if(status == G_STATUS_FAIL) {
        log_error(APP_TAG, "Cannot register player", -1);
        return G_STATUS_FAIL;
    }
    log_info(APP_TAG, "Registered player");

    status = start_menu_set_active();
    if(status == G_STATUS_FAIL) {
        log_error(APP_TAG, "Cannot activate start menu", -1);
        return G_STATUS_FAIL;
    }
    log_info(APP_TAG, "Activated start menu");


    //till here


    return G_STATUS_OK;
}

G_STATUS app_loop() {
    G_STATUS status = monitor_get_run_cond();
    // SDL_Renderer* eng_renderer = monitor_get_renderer_instance();
    // SDL_Window* eng_display = monitor_get_display_instance();

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