/*
###########################################################

@file: player.h

@author: Tanasescu Darius

@Licence: Apache2.0

@Copyleft the-nightraven, product is open source and has
absolutely no warranty, you are free to copy, distribute
and change, but not for commercial use

@Revision: InfDev

@Description: player constants

###########################################################
*/

#ifndef PLAYER_H
#define PLAYER_H

#include "app/includes.h"

//defines
#define PLAYER_SIZE_W          50
#define PLAYER_SIZE_H          50
#define PLAYER_BASE_SPEED      300 //arbitrary value
#define PLAYER_START_X         WINDOW_WIDTH / 2 - PLAYER_SIZE_W / 2
#define PLAYER_START_Y         WINDOW_HEIGHT / 2 - PLAYER_SIZE_H / 2

typedef struct Player_Typedef {
    float x;
    float y;
    float w;
    float h;
    float speed;
};

//extern functions like render
extern void draw_player();

//functions
G_STATUS init_player();
void player_move_left(void *val);
void player_move_right(void *val);
void player_stop_move(void *val);
void player_render_cb();
Player_Typedef* get_player_instance();

//updater register callbacks
void process_player_movement(void* player_instance);


#endif