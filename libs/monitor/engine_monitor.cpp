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
#include "pthread.h"

//vars
MonitorComponents_TypeDef engine_components;

//externs
RenderEngine renderer_get_engine() {
    return RENDERER_ENGINE_SDL2;
}

void* debugger_thread_lifecycle(void* args) {
    while(engine_components.debug_module.th_isRunning) {
        printf("AA\n");
    }
    return NULL;
}

G_STATUS debugger_start_th() {
    log_info(MON_TAG, "Got command to start debug");
    return monitor_start_debug();
    
}
extern G_STATUS debugger_stop_th() {
    log_info(MON_TAG, "Got command to stop debug");
    return monitor_stop_debug();
}



void* updater_thread_lifecycle(void* arg) {
    int status;
    while(engine_components.updater_module.th_isRunning) {
        status = updater_run_time_delta();

        if(status == G_STATUS_FAIL) {
            log_error(MON_TAG, "Updater thread encountered problems", -1);
            engine_components.updater_module.th_isRunning = false;
            engine_components.updater_module.status = false;
            engine_components.updater_module.upd_thread = 0;
        }
    }
    return NULL;
}

//sys callbacks
void end_game(void *val) {
    *((int*)(val)) = false;
}

G_STATUS monitor_init() {
    //engine
    engine_components.engine_display = NULL;
    engine_components.engine_renderer = NULL;
    engine_components.isRunning = false;

    //modules
    engine_components.event_module.status = false;

    engine_components.updater_module.status = false;
    engine_components.updater_module.th_isRunning = false;
    engine_components.updater_module.upd_thread = 0;

    engine_components.renderer_module.status = false;

    engine_components.debug_module.status = false;
    engine_components.debug_module.th_isRunning = false;
    engine_components.debug_module.dbg_thread = 0;

    engine_components.window_module.status = false;

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
    status = debugger_init(&engine_components.debug_module);
    if(status == G_STATUS_FAIL) {
        return status;
    }
    status = debugger_register_events();
    if(status == G_STATUS_FAIL) {
        return status;
    }
    engine_components.debug_module.status = true;
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
    //thread joins
    monitor_stop_updating();
    log_info(MON_TAG, "Unforked updater thread safely");

    //evt deinit
    evt_deinit();
    log_info(MON_TAG, "Event module deinited successfuly");

    //updater
    updater_deinit();
    log_info(MON_TAG, "Updater module deinited successfuly");

    //render deinit
    renderer_deinit();
    log_info(MON_TAG, "Render module deinited succesfully");

    //debug deinit
    debugger_deinit();
    log_info(MON_TAG, "Debugger module deinited successfully");

    //window deinit
    deinit_window(&engine_components.engine_display, &engine_components.engine_renderer);
    log_info(MON_TAG, "Window deinited successfully");

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

G_STATUS monitor_process_loop() {
    G_STATUS status;
    SDL_Event e;
    engine_components.engine_event_pool = e;
    
    status = poll_events(&engine_components.engine_event_pool);

    if(status == G_STATUS_FAIL) {
        engine_components.isRunning = false;
        log_error(MON_TAG, "cannot poll event", G_STATUS_FAIL);
        return G_STATUS_FAIL;
    }

    //render
    SDL_SetRenderDrawColor(engine_components.engine_renderer, 0, 0, 0, 255);
    SDL_RenderClear(engine_components.engine_renderer);

    renderer_create_frame(&engine_components.engine_renderer);

    SDL_RenderPresent(engine_components.engine_renderer);

    return G_STATUS_OK;
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

G_STATUS monitor_start_debug() {
    if(engine_components.debug_module.th_isRunning) {
        return G_STATUS_FAIL;
    }

    engine_components.debug_module.th_isRunning = true;
    if(pthread_create(&engine_components.debug_module.dbg_thread, NULL, debugger_thread_lifecycle, NULL) == 0) {
        return G_STATUS_OK;
    }

    engine_components.debug_module.dbg_thread = 0;
    engine_components.debug_module.th_isRunning = false;
    engine_components.debug_module.status = false;
    return G_STATUS_FAIL;
}

G_STATUS monitor_stop_debug() {
    if(!engine_components.debug_module.th_isRunning) {
        log_error(MON_TAG, "Thread wasnt running", -1);
        return G_STATUS_FAIL;
    }

    engine_components.debug_module.th_isRunning = false;
    if(pthread_join(engine_components.debug_module.dbg_thread, NULL) != 0) {
        log_error(MON_TAG, "Thread exited unsfely", -1);
        return G_STATUS_FAIL;
    }
    log_info(MON_TAG, "Thread joined safely");

    engine_components.debug_module.dbg_thread = 0;
    return G_STATUS_OK;
}

G_STATUS monitor_start_updating() {
    if(engine_components.updater_module.th_isRunning) {
        return G_STATUS_FAIL;
    }

    engine_components.updater_module.th_isRunning = true;
    if(pthread_create(&(engine_components.updater_module.upd_thread), NULL, updater_thread_lifecycle, NULL) == 0) {
        return G_STATUS_OK;
    }

    engine_components.updater_module.upd_thread = 0;
    engine_components.updater_module.th_isRunning = false;
    engine_components.updater_module.status = false;
    return G_STATUS_FAIL;
}

G_STATUS monitor_stop_updating() {
    if(!engine_components.updater_module.th_isRunning) {
        log_error(MON_TAG, "Thread wasnt running", -1);
        return G_STATUS_FAIL;
    }

    engine_components.updater_module.th_isRunning = false;
    if(pthread_join(engine_components.updater_module.upd_thread, NULL) != 0) {
        log_error(MON_TAG, "Thread exited unsfely", -1);
        return G_STATUS_FAIL;
    }
    log_info(MON_TAG, "Thread joined safely");

    engine_components.updater_module.upd_thread = 0;
    return G_STATUS_OK;
}