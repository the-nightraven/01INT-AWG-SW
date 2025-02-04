/*
###########################################################

@file: start_menu.cpp

@author: Tanasescu Darius

@Licence: Apache2.0

@Copyleft the-nightraven, product is open source and has
absolutely no warranty, you are free to copy, distribute
and change, but not for commercial use

@Revision: InfDev

@Description: Start menu scene config header

###########################################################
*/

#ifndef START_MENU_H
#define START_MENU_H

#include "includes.h"
#include "engine_monitor.h"

#define START_MENU_TAG              "STRT"

//config
#define BUTTON_PADDING              2

#define MENU_START_X                (float)(WINDOW_WIDTH/2 - 150)
#define MENU_START_Y                (float)(WINDOW_HEIGHT/2 - 210/2)
#define MENU_START_W                300
#define MENU_START_H                50
#define MENU_START_TEXT             "Start Game"

#define MENU_OPTIONS_X              (float)(WINDOW_WIDTH/2 - 150)
#define MENU_OPTIONS_Y              (float)(WINDOW_HEIGHT/2 - 210/2 + 80)
#define MENU_OPTIONS_W              300
#define MENU_OPTIONS_H              50
#define MENU_OPTIONS_TEXT           "Options"

#define MENU_EXIT_X                 (float)(WINDOW_WIDTH/2 - 150)
#define MENU_EXIT_Y                 (float)(WINDOW_HEIGHT/2 - 210/2 + 160)
#define MENU_EXIT_W                 300
#define MENU_EXIT_H                 50
#define MENU_EXIT_TEXT              "Exit"


//typedefs
typedef struct StartMenu_TypeDef {
    AWG_Rect start;
    AWG_Rect options;
    AWG_Rect exit;
} StartMenu_TypeDef;


G_STATUS init_start_menu();
G_STATUS deinit_start_menu();

G_STATUS register_start_menu();
G_STATUS start_menu_set_active();

//updater callbacks
void start_press(void* obj);
void start_hover_in(void* obj);
void start_hover_out(void* obj);

void options_press(void* obj);
void options_hover_in(void* obj);
void options_hover_out(void* obj);

void exit_press(void* obj);
void exit_hover_in(void* obj);
void exit_hover_out(void* obj);

void start_menu_render(void* obj, SDL_Renderer** renderer);

#endif //START_MENU_H
