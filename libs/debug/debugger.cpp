/*
###########################################################

@file: debugger.h

@author: Tanasescu Darius

@Licence: Apache2.0

@Copyleft the-nightraven, product is open source and has
absolutely no warranty, you are free to copy, distribute
and change, but not for commercial use

@Revision: InfDev

@Description: Debugger component source file

###########################################################
*/

#include "debugger.h"
#include "pthread.h"
#include "event_reader/reader.h"
#include "logger/logger.h"

DebugModule_TypeDef* self_module;

G_STATUS debugger_init(DebugModule_TypeDef* dbg) {
    if(dbg == NULL) {
        return G_STATUS_FAIL;
    }

    self_module = dbg;
    return G_STATUS_OK;
}

G_STATUS debugger_deinit() {
    if(self_module->th_isRunning) {
        return G_STATUS_FAIL;
    }

    return G_STATUS_OK;
}

G_STATUS debugger_register_events() {
    G_STATUS status;

    UpdateCallback_TypeDef toggle_debug = {false, &self_module->th_isRunning, debugger_toggle_cb};
    UpdateCallback_TypeDef evt_upd_stack;
    UpdateCallback_TypeDef rnd_stack;

    KeyEvt_TypeDef toggle_debug_evt = {SDL_KEYDOWN, DEBUGGER_KEY, toggle_debug, false};

    status = register_key_event(&toggle_debug_evt);
    if(status == G_STATUS_FAIL) {
        log_error(DBG_TAG, "Cannot register key event", -1);
        return G_STATUS_FAIL;
    }
    log_info(DBG_TAG, "Registered key event for DEBUG on F3");

    return G_STATUS_OK;
}

//thread lifecycle
void* debugger_lifecycle(void* arg) {

    return NULL;
}

//callbacks
void debugger_toggle_cb(void* value) {
    bool state = *(bool*)value;

    if(state) {
        debugger_stop_th();
        return;
    }
    debugger_start_th();
    return;
}

void debugger_print_evt(void* value) {
    return;
}

void debugger_print_rndr(void* value) {
    return;
}