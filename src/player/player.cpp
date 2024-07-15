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
#include "player.h"
#include "updater/updater.h"

Player_Typedef player;
bool is_init = false;

//functions
G_STATUS init_player() {
    player.w = 50;
    player.h = 50;
    player.x = 0;
    player.y = WINDOW_HEIGHT / 2 - player.h / 2;
    player.speed = 300;
    is_init = true;
    //register keys and renderer
    return G_STATUS_OK;
}

void player_move_left(void* val) {
    Player_Typedef *pu = (Player_Typedef*)val;
    pu->x -= pu->speed * updater_get_delta_time();
    printf("Should move left \n");
    return;
}

void player_move_right(void* val) {
    Player_Typedef *pu = (Player_Typedef*)val;
    pu->x += pu->speed * updater_get_delta_time();
    printf("Should move left \n");
    return;
}

void player_render_cb() {
    return;
}

Player_Typedef* get_player_instance() {
    if(!is_init) {
        return NULL;
    }

    return &player;
}