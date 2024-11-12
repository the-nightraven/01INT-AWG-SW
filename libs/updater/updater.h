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

G_STATUS updater_init();
G_STATUS updater_deinit();

bool check_updater_flag(UpdateCallback_TypeDef target);
G_STATUS updater_run_time_delta();
G_STATUS call_updater(UpdateCallback_TypeDef *target);
G_STATUS update_sys_events(SysEvtItem_TypeDef *list);
G_STATUS update_key_events(KeyEvtItem_TypeDef *list);

G_STATUS register_update_components(UpdateComponent_Typedef component);
UpdateComponent_Typedef* updater_component_to_instance(UpdateComponent_Typedef item);

//helpers
float updater_get_delta_time();

//externs
extern void* updater_thread_lifecycle(void* arg);

#endif