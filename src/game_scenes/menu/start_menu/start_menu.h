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
#define BUTTON_PADDING                  20
#define BUTTON_HEIGHT                   67
#define BUTTON_WIDTH                    200

#define MENU_START_W                    BUTTON_WIDTH
#define MENU_START_H                    BUTTON_HEIGHT
#define MENU_START_X                    (float)(WINDOW_WIDTH/2 - MENU_START_W/2)
#define MENU_START_Y                    (float)(WINDOW_HEIGHT/2 - (3*BUTTON_HEIGHT + 2 * BUTTON_PADDING) / 2)
#define MENU_START_TEXT                 "Start Game"
#define MENU_START_HOVER_IN_IND         3
#define MENU_START_HOVER_OUT_IND        2

#define MENU_OPTIONS_W                  BUTTON_WIDTH
#define MENU_OPTIONS_H                  BUTTON_HEIGHT
#define MENU_OPTIONS_X                  (float)(WINDOW_WIDTH/2 - MENU_OPTIONS_W/2)
#define MENU_OPTIONS_Y                  (float)(MENU_START_Y + MENU_START_H + BUTTON_PADDING)
#define MENU_OPTIONS_TEXT               "Options"
#define MENU_OPTIONS_HOVER_IN_IND       5
#define MENU_OPTIONS_HOVER_OUT_IND      4

#define MENU_EXIT_W                     BUTTON_WIDTH
#define MENU_EXIT_H                     BUTTON_HEIGHT
#define MENU_EXIT_X                     (float)(WINDOW_WIDTH/2 - MENU_EXIT_W/2)
#define MENU_EXIT_Y                     (float)(MENU_OPTIONS_Y + MENU_OPTIONS_H + BUTTON_PADDING)
#define MENU_EXIT_TEXT                  "Exit"
#define MENU_EXIT_HOVER_IN_IND         11
#define MENU_EXIT_HOVER_OUT_IND        10


//typedefs
typedef struct StartMenu_TypeDef {
    AWG_Rect start;
    AWG_Rect options;
    AWG_Rect exit;
    bool st_hover;
    bool op_hover;
    bool ex_hover;
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

void start_button_render(void* obj, SDL_Renderer** renderer, SDL_Texture* texture);
void options_button_render(void* obj, SDL_Renderer** renderer, SDL_Texture* texture);
void exit_button_render(void* obj, SDL_Renderer** renderer, SDL_Texture* texture);

#endif //START_MENU_H
