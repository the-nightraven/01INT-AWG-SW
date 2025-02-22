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

#include "includes.h"

//defines
#define PLAYER_TAG             "PLYR"
#define PLAYER_SIZE_W          (float)100
#define PLAYER_SIZE_H          (float)100
#define PLAYER_BASE_SPEED      (float)300 //arbitrary value
#define PLAYER_START_X         (float)WINDOW_WIDTH / 2 - PLAYER_SIZE_W / 2
#define PLAYER_START_Y         (float)WINDOW_HEIGHT / 2 - PLAYER_SIZE_H / 2

//player sprite definitions
#define PLAYER_ANIMATION_FRAME_RATIO            8

#define PLAYER_STATE_IDLE                       0
#define PLAYER_STATE_IDLE_INDEX                 0
#define PLAYER_STATE_IDLE_LENGTH                4

#define PLAYER_STATE_MOVING                     1
#define PLAYER_STATE_MOVING_INDEX               4
#define PLAYER_STATE_MOVING_LENGTH              8


#define PLAYER_SPRITE_FRAME_WIDTH               48
#define PLAYER_SPRITE_FRAME_HEIGHT              48

typedef struct Player_Typedef {
    RendererComponentHandler rnd_handler;
    AWG_Rect dimensions;
    float speed;

    //animations
    int curr_state;
    int frame_width;
    int frame_height;
    int frame_index;
    int frame_length;
    int frame_counter;

    bool orientation; //0 left, 1 right
} Player_Typedef;

//functions
G_STATUS init_player();
Player_Typedef* get_player_instance();

//player movement
void player_move_left(void *val);
void player_move_right(void *val);
void player_stop_move_left(void *val);
void player_stop_move_right(void *val);
G_STATUS player_register_events();

//updater register callbacks
void process_player_movement(void* player_instance);

//renderer callback
void player_render_cb(void* player_ins, SDL_Renderer** renderer, SDL_Texture* texture);
void player_animations_statemachine(void* state);

//test
void player_click_cb(void* val);
void player_hover_in_cb(void* val);
void player_hover_out_cb(void* val);
G_STATUS player_set_active();

#endif