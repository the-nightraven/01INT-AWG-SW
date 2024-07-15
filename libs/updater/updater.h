/*
###########################################################

@file: updater.h

@author: Tanasescu Darius

@Licence: Apache2.0

@Copyleft the-nightraven, product is open source and has
absolutely no warranty, you are free to copy, distribute
and change, but not for commercial use

@Revision: InfDev

@Description: updater logic functionality for game loop
header

###########################################################
*/

#ifndef UPDATER_H
#define UPDATER_H

#include "app/includes.h"
#include "event_reader/reader.h"

bool check_updater_flag(UpdateCallback_TypeDef target);
G_STATUS updater_run_time_delta();
G_STATUS call_updater(UpdateCallback_TypeDef *target);
G_STATUS update_sys_events(SysEvtItem_TypeDef *list);
G_STATUS update_key_events(KeyEvtItem_TypeDef *list);

float updater_get_delta_time();

#endif