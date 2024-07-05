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

SysEvt_TypeDef sys_events[1];

G_STATUS init() {
    return G_STATUS_OK;
}

G_STATUS register_sys_event(SysEvt_TypeDef* evt) {
    return G_STATUS_OK;
}

G_STATUS register_key_enevnt(KeyEvt_TypeDef* evt){
    return G_STATUS_OK;
}

G_STATUS poll_events() {
    return G_STATUS_OK;
}
