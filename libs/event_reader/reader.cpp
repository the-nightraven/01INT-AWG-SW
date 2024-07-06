/*
###########################################################

@file: reader.cpp

@author: Tanasescu Darius

@Licence: Apache2.0

@Copyleft the-nightraven, product is open source and has
absolutely no warranty, you are free to copy, distribute
and change, but not for commercial use

@Revision: InfDev

@Description: event reader

###########################################################
*/

#include "reader.h"

//TODO: dinamically hold data
SysEvt_TypeDef sys_events[10];
KeyEvt_TypeDef key_down_events[10];
KeyEvt_TypeDef key_up_events[10];
int sys_evt_len = 0;
int key_down_evt_len = 0;
int key_up_evt_len = 0;

G_STATUS init() {
    return G_STATUS_OK;
}

G_STATUS register_sys_event(SysEvt_TypeDef* evt) {
    if(evt == NULL) {
        return G_STATUS_FAIL;
    }

    sys_events[sys_evt_len] = *evt;
    sys_evt_len++;
    return G_STATUS_OK;
}

G_STATUS register_key_event(KeyEvt_TypeDef* evt){
    if(evt == NULL) {
        return G_STATUS_FAIL;
    }

    if((*evt).SDL_Hook == SDL_KEYDOWN) {
        key_down_events[key_down_evt_len++] = *evt;
    }else if((*evt).SDL_Hook == SDL_KEYUP) {
        key_up_events[key_up_evt_len++] = *evt;
    }else {
        return G_STATUS_FAIL;
    }


    return G_STATUS_OK;
}

G_STATUS poll_events(SDL_Event* e) {
    while(SDL_PollEvent(e)) {
        if((*e).type == SDL_KEYDOWN) {
            for(int i = 0; i < key_down_evt_len; i++) {
                if(key_down_events[i].Key == (*e).key.keysym.scancode) {
                    key_down_events[i].callback(key_down_events[i].value);
                }
            }
        }else if((*e).type == SDL_KEYDOWN) {
            for(int i = 0; i < key_up_evt_len; i++) {
                if(key_up_events[i].Key == (*e).key.keysym.scancode) {
                    key_up_events[i].callback(key_up_events[i].value);
                }
            }
        }else {
            for(int i = 0; i < sys_evt_len; i++) {
                if(sys_events[i].SDL_Hook == (*e).type) {
                    sys_events[i].callback(sys_events[i].value);
                }
            }
        }
    }
    return G_STATUS_OK;
}
