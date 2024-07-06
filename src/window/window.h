/*
###########################################################

@file:window.h

@author: Tanasescu Darius

@Licence: Apache2.0

@Copyleft the-nightraven, product is open source and has
absolutely no warranty, you are free to copy, distribute
and change, but not for commercial use

@Revision: InfDev

@Description: window handler

###########################################################
*/

#ifndef WINDOW_H
#define WINDOW_H

#include "app/includes.h"
//defines
#define WINDOW_WIDTH            640
#define WINDOW_HEIGHT           480
#define WINDOW_TAG              "WNDW"

//fucntions
G_STATUS init_window(SDL_Window **mainFrame, SDL_Renderer **renderer);
G_STATUS deinit_window(SDL_Window **mainFrame, SDL_Renderer **renderer);

// SDL_Window* get_window_instance();
// SDL_Renderer* get_renderer_instance();

#endif