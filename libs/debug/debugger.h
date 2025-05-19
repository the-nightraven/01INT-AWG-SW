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


#include "includes.h"

#define DBG_TAG                           "DBG"

//debug mouse stack colors
#define DBG_MOUSE_ALL_R                   255
#define DBG_MOUSE_ALL_G                   0
#define DBG_MOUSE_ALL_B                   255

#define DBG_MOUSE_HOVER_R                 0
#define DBG_MOUSE_HOVER_G                 0
#define DBG_MOUSE_HOVER_B                 255

#define DBG_MOUSE_CLICK_R                 255
#define DBG_MOUSE_CLICK_G                 0
#define DBG_MOUSE_CLICK_B                 0


#define DEBUGGER_KEY                      SDL_SCANCODE_F3
#define DEBUGGER_EVT_KEY                  SDL_SCANCODE_E
#define DEBUGGER_RND_KEY                  SDL_SCANCODE_R

#define DEBUGGER_KEY_EVT                  1
#define DEBUGGER_SYS_EVT                  2
#define DEBUGGER_MOUSE_EVT                3

#define DEBUGGER_SYS_EVT_WILDCARD         10
#define DEBUGGER_KEY_EVT_WILDCARD         11
#define DEBUGGER_MOUSE_EVT_WILDCARD       12

G_STATUS debugger_init(DebugModule_TypeDef* dbg);
G_STATUS debugger_deinit();

G_STATUS debugger_register_events();

//thread lifecycle
DWORD WINAPI debugger_lifecycle(LPVOID lpParam);
extern G_STATUS debugger_start_th();
extern G_STATUS debugger_stop_th();

//TODO: event callbacks
void debugger_toggle_cb(void* value);
void debugger_print_evt(void* value);
void debugger_print_rndr(void* value);

//externs
extern G_STATUS debugger_register_event(void* evt, int wildcard);
extern int debugger_register_to_renderer(void* obj);
extern G_STATUS debugger_unregister_to_renderer(void* obj);
extern int debugger_calc_fps();
extern void* debugger_get_evtstack_instance(int wildcard);
extern void* debugger_get_rndrstack_instance();
extern SDL_Renderer* debugger_get_renderer_instance();

//render
void dbg_render(void* obj, SDL_Renderer** renderer, SDL_Texture* texture);



#endif