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

#include "includes.h"
#include "player.h"
#include "updater.h"
#include "reader.h"
#include "logger.h"

Player_Typedef player;
bool is_init = false;

//functions
G_STATUS init_player() {
    player.dimensions.w = PLAYER_SIZE_W;
    player.dimensions.h = PLAYER_SIZE_H;
    player.dimensions.x = PLAYER_START_X;
    player.dimensions.y = PLAYER_START_Y;
    player.speed = 0;
    is_init = true;
    //register keys and renderer
    return G_STATUS_OK;
}

G_STATUS player_register_events() {
    G_STATUS status;

    UpdateCallback_TypeDef pl_move_r = {false, &player, player_move_right};
    UpdateCallback_TypeDef pl_move_l = {false, &player, player_move_left};
    UpdateCallback_TypeDef pl_move_r_stop = {false, &player, player_stop_move_right};
    UpdateCallback_TypeDef pl_move_l_stop = {false, &player, player_stop_move_left};

    KeyEvt_TypeDef pl_move_r_evt = {SDL_SCANCODE_D, pl_move_r, pl_move_r_stop, true};
    KeyEvt_TypeDef pl_move_l_evt = {SDL_SCANCODE_A, pl_move_l, pl_move_l_stop, true};

    status = register_key_event(&pl_move_l_evt);
    if(status == G_STATUS_FAIL) {
        log_error(PLAYER_TAG, "Cannot regsiter key event", G_STATUS_FAIL);
        return G_STATUS_FAIL;
    }
    log_info(PLAYER_TAG, "Registered key event on A");

    status = register_key_event(&pl_move_r_evt);
    if(status == G_STATUS_FAIL) {
        log_error(PLAYER_TAG, "Cannot regsiter key event", G_STATUS_FAIL);
        return G_STATUS_FAIL;
    }
    log_info(PLAYER_TAG, "Registered key event on D");

    UpdateComponent_Typedef player_movement = {get_player_instance(), process_player_movement};
    status = register_update_components(player_movement);
    if(status == G_STATUS_FAIL) {
        log_error(PLAYER_TAG, "Cannot register player movement cb", -1);
        return G_STATUS_FAIL;
    }
    log_info(PLAYER_TAG, "Registered player movement callback");

    //test click
    UpdateCallback_TypeDef pl_click = {false, nullptr, player_click_cb};
    MouseEvt_TypeDef pl_click_evt = {&player.dimensions, DEFAULT_UPDATER_CB, DEFAULT_UPDATER_CB, pl_click};
    status = register_mouse_event(&pl_click_evt);
    if(status == G_STATUS_FAIL) {
        log_error(PLAYER_TAG, "Cannot register mouse event", G_STATUS_FAIL);
        return G_STATUS_FAIL;
    }
    log_info(PLAYER_TAG, "Registered mouse event on click");

    return G_STATUS_OK;
}

void player_move_left(void* val) {
    auto *pu = static_cast<Player_Typedef *>(val);
    if(pu->speed > -1 * PLAYER_BASE_SPEED) {
        pu->speed -= PLAYER_BASE_SPEED;
    }
}

void player_move_right(void* val) {
    auto *pu = static_cast<Player_Typedef *>(val);
    if(pu->speed < PLAYER_BASE_SPEED) {
        pu->speed += PLAYER_BASE_SPEED;
    }
}

void player_stop_move_left(void *val) {
    auto* pu = static_cast<Player_Typedef *>(val);
    pu->speed += PLAYER_BASE_SPEED;
}

void player_stop_move_right(void *val) {
    auto* pu = static_cast<Player_Typedef *>(val);
    pu->speed -= PLAYER_BASE_SPEED;
}

Player_Typedef* get_player_instance() {
    if(!is_init) {
        return nullptr;
    }

    return &player;
}

void process_player_movement(void* player_instance) {
    auto* pl = static_cast<Player_Typedef *>(player_instance);
    pl->dimensions.x += pl->speed * updater_get_delta_time();
    //pl->x += (pl->x * updater_get_delta_time())/pl->speed;
}

void player_render_cb(void* player_ins, SDL_Renderer** renderer) {
    int status;
    auto* pl = static_cast<Player_Typedef *>(player_ins);
    SDL_FRect player_rect = {pl->dimensions.x, pl->dimensions.y, pl->dimensions.w, pl->dimensions.h};
    status = SDL_SetRenderDrawColor(*renderer, 255, 255, 255, 255);
    status = SDL_RenderDrawRectF(*renderer, &player_rect);
}

void player_click_cb(void* val) {
    printf("Clicked player\n");
}