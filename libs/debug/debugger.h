/*
###########################################################

@file: debugger.h

@author: Tanasescu Darius

@Licence: Apache2.0

@Copyleft the-nightraven, product is open source and has
absolutely no warranty, you are free to copy, distribute
and change, but not for commercial use

@Revision: InfDev

@Description: Debugger component header file

###########################################################
*/

#ifndef DEBUGGER_H
#define DEBUGGER_H


#include "app/includes.h"

#define DBG_TAG                    "DBG"

#define DEBUGGER_KEY               SDL_SCANCODE_F3
#define DEBUGGER_EVT_KEY           SDL_SCANCODE_F
#define DEBUGGER_RND_KEY           SDL_SCANCODE_R

#define DEBUGGER_KEY_EVT           1
#define DEBUGGER_SYS_EVT           2

G_STATUS debugger_init(DebugModule_TypeDef* dbg);
G_STATUS debugger_deinit();

G_STATUS debugger_register_events();

//thread lifecycle
void* debugger_lifecycle(void* arg);
extern G_STATUS debugger_start_th();
extern G_STATUS debugger_stop_th();

//TODO: event callbacks
void debugger_toggle_cb(void* value);
void debugger_print_evt(void* value);
void debugger_print_rndr(void* value);

//helpers
//TODO: get fps
int debugger_get_fps();
//TODO: get lists
void* debugger_get_list(int wildcard);
//TODO: display functions
void debugger_show_list(int wildcard);

extern G_STATUS debugger_register_event(void* evt, int wildcard);

//render
void dbg_render(void* obj, SDL_Renderer** renderer);



#endif