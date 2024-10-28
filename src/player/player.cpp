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
    player.w = PLAYER_SIZE_W;
    player.h = PLAYER_SIZE_H;
    player.x = PLAYER_START_X;
    player.y = PLAYER_START_Y;
    player.speed = 0;
    is_init = true;
    //register keys and renderer
    return G_STATUS_OK;
}

void player_move_left(void* val) {
    Player_Typedef *pu = (Player_Typedef*)val;
    //pu->x -= pu->speed * updater_get_delta_time();
    if(pu->speed > -1 * PLAYER_BASE_SPEED) {
        pu->speed -= PLAYER_BASE_SPEED;
    }
    printf("Should move left\n");
    return;
}

void player_move_right(void* val) {
    Player_Typedef *pu = (Player_Typedef*)val;
    //pu->x += pu->speed * updater_get_delta_time();
    if(pu->speed < PLAYER_BASE_SPEED) {
        pu->speed += PLAYER_BASE_SPEED;
    }
    printf("Should move right\n");
    return;
}

void player_stop_move(void *val) {
    printf("Should stop movement\n");
    Player_Typedef* pu = (Player_Typedef*)val;
    pu->speed = 0;
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

void process_player_movement(void* player_instance) {
    Player_Typedef* pl = (Player_Typedef*) player_instance;

    pl->x += pl->speed * updater_get_delta_time();
    return;
}

void player_render_cb(void* player_ins, SDL_Renderer** renderer) {
    int status;
    Player_Typedef* pl = (Player_Typedef*)player_ins;
    SDL_Rect player_rect = {pl->x, pl->y, pl->h, pl->w};
    status = SDL_SetRenderDrawColor(*renderer, 255, 255, 255, 255);
    status = SDL_RenderDrawRect(*renderer, &player_rect);
    //printf("status: %d\n", player.x);
    //SDL_RenderPresent( renderer );
    return;
}