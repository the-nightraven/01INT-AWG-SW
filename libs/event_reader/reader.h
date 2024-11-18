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
#include "updater.h"

#define SYS_EVENT_FLAG             0x03
#define KEY_EVENT_FLAG             0x04

#define KEYUP_SUBFLAG              0x05
#define KEYDOWN_SUBFLAG            0x06


//reader utils
G_STATUS evt_init();
G_STATUS evt_deinit();
G_STATUS register_sys_event(SysEvt_TypeDef* evt);
G_STATUS register_key_event(KeyEvt_TypeDef* evt);

G_STATUS poll_events(SDL_Event *e);

//conatiner utils
G_STATUS add_event_item(uint8_t type_flag, void *list, void *item);
void* get_event_by_hook(uint8_t type_flag, void *list, int hook);
void* init_event_list(uint8_t type_flag, void *item);
void* get_event_list(uint8_t type_flag, uint8_t subflag);

#endif