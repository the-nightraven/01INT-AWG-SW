/*
###########################################################

@file: engine_monitor.h

@author: Tanasescu Darius

@Licence: Apache2.0

@Copyleft the-nightraven, product is open source and has
absolutely no warranty, you are free to copy, distribute
and change, but not for commercial use

@Revision: InfDev

@Description: file engine monitor header defines and functions

###########################################################
*/

#ifndef ENGINE_MONITOR_H
#define ENGINE_MONITOR_H

#include "app/includes.h"
#include "logger/logger.h"
#include "window/window.h"
#include "event_reader/reader.h"
#include "updater/updater.h"
#include "renderer/renderer.h"

#define MON_TAG          "ENG"

G_STATUS monitor_init();
G_STATUS monitor_deinit();

G_STATUS monitor_init_modules();
G_STATUS monitor_audit_module(int wildcard);
G_STATUS monitor_deinit_modules();
G_STATUS monitor_register_comp();

void monitor_process_loop();

#endif