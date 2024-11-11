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
MonitorComponents_TypeDef engine_components;

//externs
RenderEngine renderer_get_engine() {
    return RENDERER_ENGINE_SDL2;
}

//sys callbacks
void end_game(void *val) {
    *((int*)(val)) = false;
}

G_STATUS monitor_init() {
    engine_components.engine_display = NULL;
    engine_components.engine_renderer = NULL;

    engine_components.event_module.status = false;
    engine_components.updater_module.status = false;
    engine_components.renderer_module.status = false;
    engine_components.debug_module.status = false;
    engine_components.window_module.status = false;
    engine_components.isRunning = false;
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
    engine_components.updater_module.status = true;

    //init event reader
    status = evt_init();
    if(status != G_STATUS_OK) {
        return status;
    }
    //feed into component list
    engine_components.event_module.status = true;
    //register system events
    status = monitor_register_comp();
    if(status == G_STATUS_FAIL) {
        return status;
    }

    //init renderer
    status = renderer_init();
    if(status != G_STATUS_OK) {
        return status;
    }
    //feed into component list
    engine_components.renderer_module.status = true;

    //init debug
    return G_STATUS_OK;
}

G_STATUS monitor_init_window_module() {
    G_STATUS status;
    //init window
    status = init_window(&(engine_components.engine_display), &(engine_components.engine_renderer));
    if(status == G_STATUS_FAIL) {
        return G_STATUS_FAIL;
    }
    //feed into component list
    engine_components.window_module.status = true;
    return G_STATUS_OK;
}

G_STATUS monitor_check_env() {
    if(engine_components.event_module.status && 
       engine_components.updater_module.status && 
       engine_components.renderer_module.status && 
       engine_components.window_module.status) {
        engine_components.isRunning = true;
        return G_STATUS_OK;
    }
    return G_STATUS_FAIL;
}

int monitor_audit_module(int wildcard) {
    if(wildcard == EVT_WILDCARD) {
        return (int)engine_components.event_module.status;
    }else if(wildcard == UPD_WILDCARD) {
        return (int)engine_components.updater_module.status;
    }else if(wildcard == RND_WILDCARD) {
        return (int)engine_components.renderer_module.status;
    }else if(wildcard == WDW_WILDCARD) {
        return (int)engine_components.window_module.status;
    }else if(wildcard == DBG_WILDCARD) {
        return (int)engine_components.debug_module.status;
    }
    return -1;
}

G_STATUS monitor_deinit_modules() {
    return G_STATUS_OK;
}

G_STATUS monitor_register_comp() {
    G_STATUS status;
    UpdateCallback_TypeDef sysExit = {false, &engine_components.isRunning, end_game};
    SysEvt_TypeDef sysExit_evt = {SDL_QUIT, sysExit};
    status = register_sys_event(&sysExit_evt);

    if(status == G_STATUS_FAIL) {
        log_error(MON_TAG, "Cannot register event", G_STATUS_FAIL);
        return G_STATUS_FAIL;
    }
    log_info(MON_TAG, "registered QUIT event");
    return G_STATUS_OK;
}

void monitor_process_loop() {
    return;
}

SDL_Renderer* monitor_get_renderer_instance() {
    return engine_components.engine_renderer;
}

SDL_Window* monitor_get_display_instance() {
    return engine_components.engine_display;
}

bool monitor_get_run_cond() {
    return engine_components.isRunning;
}

void monitor_force_exit() {
    engine_components.isRunning = false;
}