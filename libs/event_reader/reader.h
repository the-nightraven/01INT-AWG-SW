/*
###########################################################

@file: reader.h

@author: Tanasescu Darius

@Licence: Apache2.0

@Copyleft the-nightraven, product is open source and has
absolutely no warranty, you are free to copy, distribute
and change, but not for commercial use

@Revision: InfDev

@Description: event reader header

###########################################################
*/

#ifndef READER_H
#define READER_H

#include "includes.h"

#define SYS_EVENT_FLAG             0x03
#define KEY_EVENT_FLAG             0x04
#define MOUSE_EVENT_FLAG           0x05

#define KEYUP_SUBFLAG              0x06
#define KEYDOWN_SUBFLAG            0x07

//reader utils
G_STATUS evt_init();
G_STATUS evt_deinit();
G_STATUS register_sys_event(SysEvt_TypeDef* evt);
G_STATUS register_key_event(KeyEvt_TypeDef* evt);
G_STATUS register_mouse_event(MouseEvt_TypeDef* evt);

G_STATUS poll_events(SDL_Event *e);

//conatiner utils
G_STATUS add_event_item(uint8_t type_flag, void *list, void *item);
void* get_event_by_hook(uint8_t type_flag, void *list, int hook);
void* get_event_by_mpos(int x, int y);
void* init_event_list(uint8_t type_flag, void *item);
void* get_event_list(uint8_t type_flag);

//event control
G_STATUS evt_push_event(int sdl_hook, SDL_Scancode sdl_key);

//helpers
bool evt_fits_in_rect(int x, int y, AWG_Rect dim);

#endif