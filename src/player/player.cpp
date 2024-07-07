/*
###########################################################

@file: player.cpp

@author: Tanasescu Darius

@Licence: Apache2.0

@Copyleft the-nightraven, product is open source and has
absolutely no warranty, you are free to copy, distribute
and change, but not for commercial use

@Revision: InfDev

@Description: player logic

###########################################################
*/

#include "app/includes.h"


//functions
G_STATUS init_player() {
    //register keys and renderer
    return G_STATUS_OK;
}

void player_move_left(float *x, float *speed) {
    *(x) -= *(speed);
    return;
}

void player_move_right(float *x, float *speed) {
    *(x) += *(speed);
    return;
}

void player_render_cb() {
    return;
}