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
void player_render_cb();
Player_Typedef* get_player_instance();


#endif