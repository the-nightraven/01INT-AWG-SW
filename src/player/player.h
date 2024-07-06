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
    float speed;
};

G_STATUS init_player();
void player_move_left(float *speed);
void player_move_right(float *speed);


#endif