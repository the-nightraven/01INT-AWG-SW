/*
###########################################################

@file: engine_monitor.cpp

@author: Tanasescu Darius

@Licence: Apache2.0

@Copyleft the-nightraven, product is open source and has
absolutely no warranty, you are free to copy, distribute
and change, but not for commercial use

@Revision: InfDev

@Description: file engine monitor source code

###########################################################
*/

#include "engine_monitor.h"

//vars
SDL_Window* mainFrame;
SDL_Renderer* renderer;
MonitorComponent_TypeDef* engine_components;

//externs
RenderEngine renderer_get_engine() {
    return RENDERER_ENGINE_SDL2;
}

G_STATUS monitor_init() {
    engine_components = NULL;
    mainFrame = NULL;
    renderer = NULL;
    return G_STATUS_OK;
}

G_STATUS monitor_deinit() {
    return G_STATUS_OK;
}

G_STATUS monitor_init_modules() {
    G_STATUS status;
    //init updater
    status = updater_init();
    if(status != G_STATUS_OK) {
        return status;
    }
    //feed into component list

    //init event reader
    status = evt_init();
    if(status != G_STATUS_OK) {
        return status;
    }
    //feed into component list

    //init renderer
    status = renderer_init();
    if(status != G_STATUS_OK) {
        return status;
    }
    //feed into component list

    //init window
    status = init_window(&mainFrame, &renderer);
    if(status == G_STATUS_FAIL) {
        return G_STATUS_FAIL;
    }
    //feed into component list

    //init debug
    return G_STATUS_OK;
}

G_STATUS monitor_audit_module(int wildcard) {
    return G_STATUS_OK;
}

G_STATUS monitor_deinit_modules() {
    return G_STATUS_OK;
}

G_STATUS monitor_register_comp() {
    UpdateCallback_TypeDef sysExit = {false, &is_running, end_game};
    SysEvt_TypeDef sysExit_evt = {SDL_QUIT, sysExit};
    status = register_sys_event(&sysExit_evt);

    if(status == G_STATUS_FAIL) {
        log_error(APP_TAG, "Cannot register event", G_STATUS_FAIL);
        return G_STATUS_FAIL;
    }
    log_info("APP", "registered QUIT event");
}

void monitor_process_loop() {
    return;
}