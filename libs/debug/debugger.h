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

#define DEBUGGER_KEY               SDL_SCANCODE_F3
#define DEBUGGER_EVT_KEY           SDL_SCANCODE_F
#define DEBUGGER_RND_KEY           SDL_SCANCODE_R

G_STATUS debugger_init();
G_STATUS debugger_deinit();

//thread lifecycle
G_STATUS debugger_start();
G_STATUS debugger_end();
void* debugger_lifecycle(void* arg);

//TODO: event callbacks
void debugger_toggle_cb(void* value);
void debugger_calc_fps_cb(void* value);
void debugger_print_list(void* value);

//helpers
//TODO: get fps
int debugger_get_fps();
//TODO: get lists
void* debugger_get_list(int wildcard);
//TODO: display functions
void debugger_show_list(int wildcard);
bool debugger_is_running();



#endif