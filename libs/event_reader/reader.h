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

typedef struct SysEvt_TypeDef {
    int SDL_Hook;
    void (*callback);
};

typedef struct KeyEvt_TypeDef {
    int SDL_Hook;
    int Key;
    void (*callback);
};

G_STATUS init();
G_STATUS register_sys_event(SysEvt_TypeDef* evt);
G_STATUS register_key_enevnt(KeyEvt_TypeDef* evt);

G_STATUS poll_events();

#endif