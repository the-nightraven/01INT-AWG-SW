/*
###########################################################

@file:app.h

@author: Tanasescu Darius

@Licence: Apache2.0

@Copyleft the-nightraven, product is open source and has
absolutely no warranty, you are free to copy, distribute
and change, but not for commercial use

@Revision: InfDev

@Description: app definitions

###########################################################
*/

#ifndef APP_HEADER
#define APP_HEADER

#define APP_TAG       "APP"

//includes
#include "monitor/engine_monitor.h"
#include "player/player.h"

//fucntions
G_STATUS app_init();
G_STATUS app_loop();

G_STATUS app_deinit();

// void end_game(void *val);
// void test(void* val);

#endif