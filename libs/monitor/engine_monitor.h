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
#include "debug/debugger.h"

#define MON_TAG          "ENG"
#define EVT_WILDCARD     1
#define UPD_WILDCARD     2
#define RND_WILDCARD     3
#define WDW_WILDCARD     4
#define DBG_WILDCARD     5

//inits
G_STATUS monitor_init();
G_STATUS monitor_deinit();
G_STATUS monitor_init_modules();
G_STATUS monitor_init_window_module();
G_STATUS monitor_register_comp();

//testing
G_STATUS monitor_check_env();
int monitor_audit_module(int wildcard);

//deinit
G_STATUS monitor_deinit_modules();

//loop functions
G_STATUS monitor_start_debug();
G_STATUS monitor_stop_debug();
G_STATUS monitor_start_updating();
G_STATUS monitor_stop_updating();
G_STATUS monitor_process_loop();

//getters and helpers
SDL_Renderer* monitor_get_renderer_instance();
SDL_Window* monitor_get_display_instance();
bool monitor_get_run_cond();
void monitor_force_exit();

#endif