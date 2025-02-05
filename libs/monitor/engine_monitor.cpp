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
#include <chrono>

//vars
MonitorComponents_TypeDef engine_components;

//temporary until scenes has component
bool CHANGE_SCENES_FLAG = false;
auto* scene_ch_name = static_cast<char *>(malloc(sizeof(char) * 64));
int SCENE_MODE = -1;

void change_scene_rename(const char* newname, int mode) {
    CHANGE_SCENES_FLAG = true;
    strcpy(scene_ch_name, newname);
    SCENE_MODE = mode;
}

//externs
RenderEngine renderer_get_engine() {
    return RENDERER_ENGINE_SDL2;
}

#if DEBUG
SDL_Renderer* debugger_get_renderer_instance() {
    return engine_components.engine_renderer;
}

extern int debugger_register_to_renderer(void* obj) {
    return renderer_register_component(static_cast<DebugModule_TypeDef *>(obj)->rnd_comp);
}

extern G_STATUS debugger_unregister_to_renderer(void* obj) {
    return renderer_remove_component(static_cast<DebugModule_TypeDef *>(obj)->rnd_handler);
}

extern int debugger_calc_fps() {
    return monitor_micros_to_fps(engine_components.frameTime);
}

G_STATUS debugger_start_th() {
    log_info(MON_TAG, "Got command to start debug");
    return monitor_start_debug();
    
}
G_STATUS debugger_stop_th() {
    log_info(MON_TAG, "Got command to stop debug");
    return monitor_stop_debug();
}

G_STATUS debugger_register_event(void* evt, int wildcard) {
    if(wildcard == DEBUGGER_KEY_EVT) {
        return register_key_event(ENGINE_ESSENTIAL_COMPONENT, static_cast<KeyEvt_TypeDef *>(evt));
    }
    if(wildcard == DEBUGGER_SYS_EVT) {
        return register_sys_event(ENGINE_ESSENTIAL_COMPONENT, static_cast<SysEvt_TypeDef *>(evt));
    }
    return G_STATUS_FAIL;
}

void* debugger_get_evtstack_instance(int wildcard) {
    if(wildcard == DEBUGGER_SYS_EVT_WILDCARD) {
        return get_event_list(SYS_EVENT_FLAG);
    }
    if(wildcard == DEBUGGER_KEY_EVT_WILDCARD) {
        return get_event_list(KEY_EVENT_FLAG);
    }
    if(wildcard == DEBUGGER_MOUSE_EVT_WILDCARD) {
        return get_event_list(MOUSE_EVENT_FLAG);
    }
    return nullptr;
}

void* debugger_get_rndrstack_instance() {
    return renderer_get_list();
}
#endif

//add scenes module typedef
G_STATUS scene_sys_clear_components() {
    evt_clear_nonessential();
#if DEBUG
    log_debug(MON_TAG, "Cleared evt", -1);
#endif
    updater_clear_comp_nonessentials();
#if DEBUG
    log_debug(MON_TAG, "Cleared updt", -1);
#endif
    renderer_clear_stack();
#if DEBUG
    log_debug(MON_TAG, "Cleared rndr", -1);
#endif
    return G_STATUS_OK;
}

G_STATUS scene_sys_load_components(SceneComponent_TypeDef* comp_list, int mode) {

    //stop updater thread
    monitor_stop_updating();

    if(mode == SCENE_MODE_CLEAR) {
        scene_sys_clear_components();
    }

    //loop through list
    SceneComponent_TypeDef* ind = comp_list;
    G_STATUS status;

    while(ind != nullptr) {
        //register events with noness
        KeyEvtItem_TypeDef* keind = ind->key_evt_def;
        MouseEvtItem_TypeDef* meind = ind->mouse_evt_def;
        SysEvtItem_TypeDef* seind = ind->sys_evt_def;

        while(keind != nullptr) {
            printf("HERE KEY\n");
            printf("KEY SCANCODE:\n");
            status = register_key_event(ENGINE_NONESSENTIAL_COMPONENT, &keind->evt);
            if(status != G_STATUS_OK) {
                log_error(MON_TAG, "Failed to add key event", -1);
                return status;
            }
            keind = keind->next;
        }
        if(keind != nullptr) {
            log_info(MON_TAG, "Registered key events");
        }

        while(meind != nullptr) {
            printf("HERE MOUSE\n");
            status = register_mouse_event(ENGINE_NONESSENTIAL_COMPONENT, &meind->evt);
            if(status != G_STATUS_OK) {
                log_error(MON_TAG, "Failed to add mouse event", -1);
                return status;
            }
            meind = meind->next;
        }
        if(meind != nullptr) {
            log_info(MON_TAG, "Registered mouse events");
        }

        while(seind != nullptr) {
            printf("HERE SYS\n");
            status = register_sys_event(ENGINE_NONESSENTIAL_COMPONENT, &seind->evt);
            if(status != G_STATUS_OK) {
                log_error(MON_TAG, "Failed to add sys event", -1);
                return status;
            }
            seind = seind->next;
        }
        if(seind != nullptr) {
            log_info(MON_TAG, "Registered system events");
        }

        //register updater comp if needed with noess
        UpdateComponent_Typedef* ucind = ind->update_def;
        while(ucind != nullptr) {
            ucind->essential = ENGINE_NONESSENTIAL_COMPONENT; //privilege escalation guard
            status = register_update_components(*ucind);
            if(status != G_STATUS_OK) {
                log_error(MON_TAG, "Failed to add updater component", -1);
                return status;
            }
            ucind = ucind->next;
        }
        if(ucind != nullptr) {
            log_info(MON_TAG, "Registered updater components");
        }

        //register renderer components with noess
        if(ind->rnd_component != nullptr) {
            RendererComponent_Typedef* rcind = ind->rnd_component;
            renderer_register_component(*rcind);
            //loopback handler to object

            log_info(MON_TAG, "Registered renderer component");
        }


        ind = ind->next;
    }

    //start updater
    monitor_start_updating();

    //return
    return G_STATUS_OK;
}



DWORD WINAPI updater_thread_lifecycle(LPVOID lpParam) {
    while(engine_components.updater_module.th_isRunning) {
        if(const int status = updater_run_time_delta(); status == G_STATUS_FAIL) {
            log_error(MON_TAG, "Updater thread encountered problems", -1);
            engine_components.updater_module.th_isRunning = false;
            engine_components.updater_module.status = false;
            engine_components.updater_module.upd_thread = nullptr;
        }
    }
    return 0;
}

//sys callbacks
void end_game(void *val) {
    *static_cast<int *>(val) = false;
}

void force_end_game(void *val) {
    evt_push_event(SDL_QUIT, SDL_SCANCODE_UNKNOWN);
}

void toggle_fullscreen(void* val) {
    const auto fs = static_cast<bool *>(val);
    *fs = !*fs;
    if(*fs) {
        SDL_SetWindowFullscreen(engine_components.engine_display, SDL_WINDOW_FULLSCREEN_DESKTOP);
        return;
    }
    SDL_SetWindowFullscreen(engine_components.engine_display, 0);
}

G_STATUS monitor_init() {
    //engine
    engine_components.engine_display = nullptr;
    engine_components.engine_renderer = nullptr;
    engine_components.isRunning = false;
    engine_components.frameTime = 0;
    engine_components.fullScreen = false;

    //modules
    engine_components.event_module.status = false;

    engine_components.updater_module.status = false;
    engine_components.updater_module.th_isRunning = false;
    engine_components.updater_module.upd_thread = nullptr;

    engine_components.renderer_module.status = false;

    engine_components.debug_module.status = false;
    engine_components.debug_module.th_isRunning = false;
    engine_components.debug_module.dbg_thread = nullptr;
    engine_components.debug_module.rnd_handler = 0;
    engine_components.debug_module.mouseStackVisible = false;

    engine_components.window_module.status = false;

    engine_components.scenes_module.status = false;
    engine_components.scenes_module.first_scene_id = -1;
    engine_components.scenes_module.change_flag = false;
    engine_components.scenes_module.change_id = -1;
    engine_components.scenes_module.change_mode = -1;
    engine_components.scenes_module.id_pointer = -1;

    return G_STATUS_OK;
}

//TODO free memory
G_STATUS monitor_deinit() {
    return G_STATUS_OK;
}

G_STATUS monitor_init_modules() {
    //init updater
    G_STATUS status = updater_init();
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
#if DEBUG
    log_debug(MON_TAG, "Inited renderer", 0);
#endif

    //init scenes
    status = scenes_init(&engine_components.scenes_module);
    if(status == G_STATUS_FAIL) {
        return status;
    }
    engine_components.scenes_module.status = true;

    return G_STATUS_OK;
}

G_STATUS monitor_init_window_module() {
    G_STATUS status;
    //init window
    if(status = init_window(&(engine_components.engine_display), &(engine_components.engine_renderer)); status == G_STATUS_FAIL) {
        return G_STATUS_FAIL;
    }
    //feed into component list
    engine_components.window_module.status = true;

#if DEBUG
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
#endif

    return G_STATUS_OK;
}

G_STATUS monitor_check_env() {
    if(engine_components.event_module.status && 
       engine_components.updater_module.status && 
       engine_components.renderer_module.status && 
       engine_components.window_module.status &&
       engine_components.scenes_module.status) {
        engine_components.isRunning = true;
        return G_STATUS_OK;
    }
    return G_STATUS_FAIL;
}

int monitor_audit_module(int wildcard) {
    if(wildcard == EVT_WILDCARD) {
        return (int)engine_components.event_module.status;
    }

    if(wildcard == UPD_WILDCARD) {
        return (int)engine_components.updater_module.status;
    }

    if(wildcard == RND_WILDCARD) {
        return (int)engine_components.renderer_module.status;
    }

    if(wildcard == WDW_WILDCARD) {
        return (int)engine_components.window_module.status;
    }

#if DEBUG
    if(wildcard == DBG_WILDCARD) {
        return (int)engine_components.debug_module.status;
    }
#endif

    if(wildcard == SCE_WILDCARD) {
        return (int)engine_components.scenes_module.status;
    }

    return -1;
}

G_STATUS monitor_deinit_modules() {
    //thread joins
    monitor_stop_updating();
    log_info(MON_TAG, "Un forked updater thread safely");

#if DEBUG
    monitor_stop_debug();
    log_info(MON_TAG, "Un forked debug thread safely");
#endif

    //scenes de init
    scenes_deinit();
    log_info(MON_TAG, "Scenes Module de inited successfully");

    //evt de init
    evt_deinit();
    log_info(MON_TAG, "Event module de inited successfully");

    //updater
    updater_deinit();
    log_info(MON_TAG, "Updater module de inited successfully");

    //render de init
    renderer_deinit();
    log_info(MON_TAG, "Render module de inited successfully");

#if DEBUG
    //debug de init
    debugger_deinit();
    log_info(MON_TAG, "Debugger module de inited successfully");
#endif

    //window de init
    deinit_window(&engine_components.engine_display, &engine_components.engine_renderer);
    log_info(MON_TAG, "Window de inited successfully");

    return G_STATUS_OK;
}

G_STATUS monitor_register_comp() {
    constexpr UpdateCallback_TypeDef sysExit = {false, &engine_components.isRunning, end_game};
    SysEvt_TypeDef sysExit_evt = {SDL_QUIT, sysExit};
    G_STATUS status = register_sys_event(ENGINE_ESSENTIAL_COMPONENT, &sysExit_evt);

    if(status == G_STATUS_FAIL) {
        log_error(MON_TAG, "Cannot register QUIT event", G_STATUS_FAIL);
        return G_STATUS_FAIL;
    }
    log_info(MON_TAG, "registered QUIT event");

    UpdateCallback_TypeDef forcedSysExit = {false, nullptr, force_end_game};
    KeyEvt_TypeDef forcedSysExit_evt = {FORCE_QUIT_KEY, forcedSysExit, DEFAULT_UPDATER_CB, false};
    status = register_key_event(ENGINE_ESSENTIAL_COMPONENT, &forcedSysExit_evt);

    if(status == G_STATUS_FAIL) {
        log_error(MON_TAG, "Cannot register FORCE_QUIT key event", G_STATUS_FAIL);
        return G_STATUS_FAIL;
    }
    log_info(MON_TAG, "registered FORCE_QUIT key event on Q");

    UpdateCallback_TypeDef fullscreen_t = {false, &engine_components.fullScreen, toggle_fullscreen};
    KeyEvt_TypeDef fullscreen_t_evt = {FULLSCREEN_KEY, fullscreen_t, DEFAULT_UPDATER_CB, false};
    status = register_key_event(ENGINE_ESSENTIAL_COMPONENT, &fullscreen_t_evt);

    if(status == G_STATUS_FAIL) {
        log_error(MON_TAG, "Cannot register FULLSCREEN key event", G_STATUS_FAIL);
        return G_STATUS_FAIL;
    }
    log_info(MON_TAG, "registered FULLSCREEN key event on F");
    return G_STATUS_OK;
}

G_STATUS monitor_process_loop() {

    //actually change scenes here
    if(engine_components.scenes_module.change_flag) {
        scene_load(nullptr, engine_components.scenes_module.change_id, engine_components.scenes_module.change_mode);

        engine_components.scenes_module.change_flag = false;
        engine_components.scenes_module.change_id = -1;
        engine_components.scenes_module.change_mode = -1;
    }

    const std::chrono::time_point start_timer = std::chrono::high_resolution_clock::now();

    SDL_Event e;
    // ReSharper disable once CppLocalVariableMightNotBeInitialized
    engine_components.engine_event_pool = e;

    if(const G_STATUS status = poll_events(&engine_components.engine_event_pool); status == G_STATUS_FAIL) {
        engine_components.isRunning = false;
        log_error(MON_TAG, "cannot poll event", G_STATUS_FAIL);
        return G_STATUS_FAIL;
    }

    //render
    SDL_SetRenderDrawColor(engine_components.engine_renderer, 0, 0, 0, 255);
    SDL_RenderClear(engine_components.engine_renderer);

    renderer_create_frame(&engine_components.engine_renderer);

    SDL_RenderPresent(engine_components.engine_renderer);

    //cron end
    const std::chrono::time_point stop_timer = std::chrono::high_resolution_clock::now();

    //calc frame time
    const long long exec_micros = std::chrono::duration_cast<std::chrono::microseconds>(stop_timer - start_timer).count();
    engine_components.frameTime = static_cast<int>(exec_micros);

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

#if DEBUG
G_STATUS monitor_start_debug() {
    if(engine_components.debug_module.th_isRunning) {
        return G_STATUS_FAIL;
    }

    engine_components.debug_module.th_isRunning = true;

    //create thread
    engine_components.debug_module.dbg_thread = CreateThread(nullptr, 0, debugger_lifecycle, nullptr, 0, nullptr);
    if(engine_components.debug_module.dbg_thread == nullptr) {
        log_error(MON_TAG, "Cannot create debug thread", G_STATUS_FAIL);
        engine_components.debug_module.dbg_thread = nullptr;
        engine_components.debug_module.th_isRunning = false;
        engine_components.debug_module.status = false;
        return G_STATUS_FAIL;
    }
    log_info(MON_TAG, "Forked debug thread");

    //set the lowest priority
    if(!SetThreadPriority(engine_components.debug_module.dbg_thread, THREAD_PRIORITY_LOWEST)) {
        log_error(MON_TAG, "Cannot set thread priority", G_STATUS_FAIL);
        CloseHandle(engine_components.debug_module.dbg_thread);
        engine_components.debug_module.dbg_thread = nullptr;
        engine_components.debug_module.th_isRunning = false;
        engine_components.debug_module.status = false;
        return G_STATUS_FAIL;
    }
    log_info(MON_TAG, "Debugger thread started");

    return G_STATUS_OK;
}

G_STATUS monitor_stop_debug() {
    if(!engine_components.debug_module.th_isRunning) {
        log_error(MON_TAG, "Thread wasnt running", -1);
        return G_STATUS_FAIL;
    }

    engine_components.debug_module.th_isRunning = false;
    WaitForSingleObject(engine_components.debug_module.dbg_thread, INFINITE);
    CloseHandle(engine_components.debug_module.dbg_thread);
    log_info(MON_TAG, "Debugger thread stopped");

    engine_components.debug_module.dbg_thread = nullptr;
    return G_STATUS_OK;
}
#endif

G_STATUS monitor_start_updating() {
    if(engine_components.updater_module.th_isRunning) {
        return G_STATUS_FAIL;
    }

    engine_components.updater_module.th_isRunning = true;
    // if(pthread_create(&(engine_components.updater_module.upd_thread), NULL, updater_thread_lifecycle, NULL) == 0) {
    //     return G_STATUS_OK;
    // }
    engine_components.updater_module.upd_thread = CreateThread(nullptr, 0, updater_thread_lifecycle, nullptr, 0, nullptr);
    if(engine_components.updater_module.upd_thread == nullptr) {
        log_error(MON_TAG, "Cannot create updater thread", G_STATUS_FAIL);
        engine_components.updater_module.upd_thread = nullptr;
        engine_components.updater_module.th_isRunning = false;
        engine_components.updater_module.status = false;
        return G_STATUS_FAIL;
    }
    log_info(MON_TAG, "Updater thread forked");

    if(!SetThreadPriority(engine_components.updater_module.upd_thread, THREAD_PRIORITY_LOWEST)) {
        log_error(MON_TAG, "Cannot set thread priority", G_STATUS_FAIL);
        CloseHandle(engine_components.updater_module.upd_thread);
        engine_components.updater_module.upd_thread = nullptr;
        engine_components.updater_module.th_isRunning = false;
        engine_components.updater_module.status = false;
        return G_STATUS_FAIL;
    }
    log_info(MON_TAG, "Updater thread started");

    return G_STATUS_OK;
}

G_STATUS monitor_stop_updating() {
    if(!engine_components.updater_module.th_isRunning) {
        log_error(MON_TAG, "Thread wasnt running", -1);
        return G_STATUS_FAIL;
    }

    engine_components.updater_module.th_isRunning = false;
    WaitForSingleObject(engine_components.updater_module.upd_thread, INFINITE);
    CloseHandle(engine_components.updater_module.upd_thread);
    log_info(MON_TAG, "Thread joined safely");

    return G_STATUS_OK;
}

int monitor_micros_to_fps(int micros) {
    return 1000000 / micros;
}