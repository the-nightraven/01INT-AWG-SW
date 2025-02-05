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
#include "reader.h"
#include "logger.h"
#include "font_cache.h"

DebugModule_TypeDef* self_module;
FC_Font* dbg_font;

G_STATUS debugger_init(DebugModule_TypeDef* dbg) {
    if(dbg == nullptr) {
        return G_STATUS_FAIL;
    }

    self_module = dbg;
    self_module->rnd_comp = (RendererComponent_Typedef){
        0,
        "Debugger",
        true,
        nullptr,
        1,
        dbg_render,
        nullptr
    };
    self_module->fps = 0;
    dbg_font = FC_CreateFont();
    FC_LoadFont(dbg_font, debugger_get_renderer_instance(), "../assets/fonts/Lato-Regular.ttf", 20,
                FC_MakeColor(0, 255, 0, 255), TTF_STYLE_NORMAL);
    return G_STATUS_OK;
}

G_STATUS debugger_deinit() {
    if(self_module->th_isRunning) {
        return G_STATUS_FAIL;
    }
    FC_FreeFont(dbg_font);

    return G_STATUS_OK;
}

G_STATUS debugger_register_events() {
    G_STATUS status;

    UpdateCallback_TypeDef toggle_debug = {false, &self_module->th_isRunning, debugger_toggle_cb};
    UpdateCallback_TypeDef evt_upd_stack = {false, &self_module->mouseStackVisible, debugger_print_evt};
    UpdateCallback_TypeDef rnd_stack = {false, nullptr, debugger_print_rndr};

    KeyEvt_TypeDef toggle_debug_evt = {DEBUGGER_KEY, toggle_debug, DEFAULT_UPDATER_CB, false};
    KeyEvt_TypeDef print_evt_upd_stack_evt = {DEBUGGER_EVT_KEY, evt_upd_stack, DEFAULT_UPDATER_CB, false};
    KeyEvt_TypeDef print_rndr_stack_evt = {DEBUGGER_RND_KEY, rnd_stack, DEFAULT_UPDATER_CB, false};

    status = debugger_register_event((void*)&toggle_debug_evt, DEBUGGER_KEY_EVT);
    if(status == G_STATUS_FAIL) {
        log_error(DBG_TAG, "Cannot register key event", -1);
        return G_STATUS_FAIL;
    }
    log_info(DBG_TAG, "Registered key event for DEBUG on F3");

    status = debugger_register_event((void*)&print_evt_upd_stack_evt, DEBUGGER_KEY_EVT);
    if(status == G_STATUS_FAIL) {
        log_error(DBG_TAG, "Cannot register key event", -1);
        return G_STATUS_FAIL;
    }
    log_info(DBG_TAG, "Registered key event for DEBUG_EVT_STACK on E");

    status = debugger_register_event((void*)&print_rndr_stack_evt, DEBUGGER_KEY_EVT);
    if(status == G_STATUS_FAIL) {
        log_error(DBG_TAG, "Cannot register key event", -1);
        return G_STATUS_FAIL;
    }
    log_info(DBG_TAG, "Registered key event for DEBUG_RNDR_STACK on R");

    return G_STATUS_OK;
}

//thread lifecycle
DWORD WINAPI debugger_lifecycle(LPVOID lpParam) {
    //push to visible stack
    self_module->rnd_handler = debugger_register_to_renderer(self_module);
    while(self_module->th_isRunning) {
        //calculate fps and update
        self_module->fps = debugger_calc_fps();
        Sleep(500);
    }
    //pop from visible stack
    debugger_unregister_to_renderer(self_module);
    self_module->rnd_handler = 0;
    return 0;
}

//callbacks
void debugger_toggle_cb(void* value) {
    if(bool state = *static_cast<bool *>(value)) {
        debugger_stop_th();
        return;
    }
    debugger_start_th();
}

void debugger_print_evt(void* value) {
    if(!self_module->th_isRunning) {
        return;
    }
    log_info(DBG_TAG, "DEBUG DRAW EVT TRIGGERED");

    *static_cast<bool *>(value) = !*static_cast<bool *>(value);
}

void debugger_print_rndr(void* value) {
    if(!self_module->th_isRunning) {
        return;
    }

    auto* ind = static_cast<RendererComponent_Typedef*>(debugger_get_rndrstack_instance());
    while(ind != nullptr) {
        printf("--------------------------------------\n");
        printf("HANDLER: %d\n", ind->handler);
        printf("NAME: %s\n", ind->name);
        printf("VISIBILITY: %d\n", ind->visibility);
        printf("OBJ_TYPE: %d\n", ind->obj_type);
        printf("--------------------------------------\n");
        ind = ind->next;
    }
}

//@TODO: split debug rendering objects into their own components
void dbg_render(void* obj, SDL_Renderer** renderer) {
    FC_Draw(dbg_font, *renderer, 0, 0, "%d FPS\n", self_module->fps);

    if(self_module->mouseStackVisible) {
        auto *ind = static_cast<MouseEvtItem_TypeDef *>(debugger_get_evtstack_instance(DEBUGGER_MOUSE_EVT_WILDCARD));
        while(ind != nullptr) {
            int r = 0, b = 0;
            if(ind->evt.click_cb.obj_callback != nullptr) {
                r = 255;
            }
            if(ind->evt.hover_in_cb.obj_callback != nullptr) {
                b = 255;
            }

            SDL_FRect rect = {ind->evt.dim->x, ind->evt.dim->y, ind->evt.dim->w, ind->evt.dim->h};
            SDL_SetRenderDrawColor(*renderer, r, 0, b, 60);
            SDL_RenderFillRectF(*renderer, &rect);
            SDL_RenderDrawRectF(*renderer, &rect);

            ind = ind->next;
        }
    }
}