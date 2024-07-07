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

#include "app/includes.h"
#include <inttypes.h>

#define SYS_EVENT_FLAG             0x03
#define KEY_EVENT_FLAG             0x04

//events typedefs
typedef struct SysEvt_TypeDef {
    int SDL_Hook;
    void* value;
    void (*callback)(void*);
};

typedef struct KeyEvt_TypeDef {
    int SDL_Hook;
    int Key;
    void* value;
    void (*callback)(void*);
};


//datastructures typedefs
typedef struct SysEvtItem_TypeDef {
    SysEvt_TypeDef evt;
    SysEvtItem_TypeDef *next;
};

typedef struct KeyEvtItem_TypeDef {
    KeyEvt_TypeDef evt;
    KeyEvtItem_TypeDef *next;
};

//reader utils
G_STATUS init();
G_STATUS register_sys_event(SysEvt_TypeDef* evt);
G_STATUS register_key_event(KeyEvt_TypeDef* evt);

G_STATUS poll_events(SDL_Event *e);

//conatiner utils
G_STATUS add_event_item(uint8_t type_flag, void *list, void *item);
void* get_event_by_hook(uint8_t type_flag, void *list, int hook);
void* init_event_list(uint8_t type_flag, void *item);

#endif