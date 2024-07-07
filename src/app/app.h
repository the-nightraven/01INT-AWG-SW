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
#include "includes.h"
#include "logger/logger.h"
#include "window/window.h"
#include "event_reader/reader.h"
#include "updater/updater.h"

//fucntions
G_STATUS app_init();
G_STATUS app_loop();

void end_game(void *val);
void test(void* val);

#endif