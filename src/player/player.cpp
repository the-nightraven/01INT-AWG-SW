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

#include <chrono>
#include <SDL_image.h>

#include "updater.h"
#include "reader.h"
#include "logger.h"
#include "renderer.h"
#include "scene/scene.h"

Player_Typedef player;
bool is_init = false;
SceneItem_TypeDef player_scene;
bool evt_trigger = false;

//functions
G_STATUS init_player() {
    player.rnd_handler = -1;
    player.dimensions.w = PLAYER_SIZE_W;
    player.dimensions.h = PLAYER_SIZE_H;
    player.dimensions.x = PLAYER_START_X;
    player.dimensions.y = PLAYER_START_Y;
    player.speed = 0;
    player.curr_state = 0;
    player.frame_width = PLAYER_SPRITE_FRAME_WIDTH;
    player.frame_height = PLAYER_SPRITE_FRAME_HEIGHT;
    player.frame_index = PLAYER_STATE_IDLE_INDEX;
    player.frame_length = PLAYER_STATE_IDLE_LENGTH;
    player.frame_counter = 0;
    player.orientation = false;

    is_init = true;
    return G_STATUS_OK;
}

G_STATUS player_register_events() {
    G_STATUS status;

    //evt stack
    UpdateCallback_TypeDef pl_move_r = {false, &player, player_move_right};
    UpdateCallback_TypeDef pl_move_l = {false, &player, player_move_left};
    UpdateCallback_TypeDef pl_move_r_stop = {false, &player, player_stop_move_right};
    UpdateCallback_TypeDef pl_move_l_stop = {false, &player, player_stop_move_left};

    KeyEvt_TypeDef pl_move_r_evt = {SDL_SCANCODE_D, pl_move_r, pl_move_r_stop, true};
    KeyEvt_TypeDef pl_move_l_evt = {SDL_SCANCODE_A, pl_move_l, pl_move_l_stop, true};

    KeyEvtItem_TypeDef pl_mv_r_evt = {ENGINE_NONESSENTIAL_COMPONENT, pl_move_r_evt, nullptr};
    KeyEvtItem_TypeDef pl_mv_l_evt = {ENGINE_NONESSENTIAL_COMPONENT, pl_move_l_evt, nullptr};

    KeyEvtItem_TypeDef* ppmr = &pl_mv_r_evt;
    KeyEvtItem_TypeDef* ppml = &pl_mv_l_evt;

    ppmr->next = ppml;
    ppml->next = nullptr;


    //update stack
    UpdateComponent_Typedef player_movement = {ENGINE_NONESSENTIAL_COMPONENT, get_player_instance(), process_player_movement, nullptr};


    //rnd stack

    RendererComponent_Typedef player_render = {};
    player_render.handler = 0;
    player_render.name = "Player";
    player_render.visibility = true;

    player_render.sprite.map_path = "../assets/characters/main/default.png";
    player_render.sprite.texture = nullptr;

    player_render.animatable = true;
    player_render.animation_cb = {false, nullptr, player_animations_statemachine, nullptr};


    player_render.object = get_player_instance();
    player_render.obj_type = 1;
    player_render.obj_render = player_render_cb;
    player_render.next = nullptr;

    //animations

    //test click
    // UpdateCallback_TypeDef pl_click = {false, nullptr, player_click_cb};
    // UpdateCallback_TypeDef pl_hov_in = {false, nullptr, player_hover_in_cb};
    // UpdateCallback_TypeDef pl_hov_out = {false, nullptr, player_hover_out_cb};
    // MouseEvt_TypeDef pl_click_evt = {&player.dimensions, false, pl_hov_in, pl_hov_out, pl_click};
    //
    // MouseEvtItem_TypeDef mevt = {ENGINE_NONESSENTIAL_COMPONENT, pl_click_evt, nullptr};
    // MouseEvtItem_TypeDef* pmevt = &mevt;
    // pmevt->next = nullptr;

    SceneComponent_TypeDef player_comp = {&player_movement, ppmr, nullptr, nullptr, &player_render, nullptr};
    player_comp.update_def = &player_movement;
    player_comp.key_evt_def = ppmr;
    player_comp.mouse_evt_def = nullptr;
    player_comp.sys_evt_def = nullptr;
    player_comp.rnd_component = &player_render;
    player_comp.next = nullptr;


    player_scene.name = "Player";
    player_scene.active = false;
    player_scene.type = 0;
    player_scene.scene_comp_list = &player_comp;
    player_scene.next = nullptr;

    scene_add(player_scene);

    return G_STATUS_OK;
}

void player_move_left(void* val) {
    //update animation state
    evt_trigger = true;

    auto *pu = static_cast<Player_Typedef *>(val);
    if(pu->speed > -1 * PLAYER_BASE_SPEED) {
        pu->speed -= PLAYER_BASE_SPEED;
    }
}

void player_move_right(void* val) {
    //update animation state
    evt_trigger = true;

    auto *pu = static_cast<Player_Typedef *>(val);
    if(pu->speed < PLAYER_BASE_SPEED) {
        pu->speed += PLAYER_BASE_SPEED;
    }
}

void player_stop_move_left(void *val) {
    //update animation state
    evt_trigger = true;

    auto* pu = static_cast<Player_Typedef *>(val);
    pu->speed += PLAYER_BASE_SPEED;
}

void player_stop_move_right(void *val) {
    //update animation state
    evt_trigger = true;

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

    if(evt_trigger) {
        if(pl->speed == 0) {
            player.curr_state = PLAYER_STATE_IDLE;
            player.frame_index = PLAYER_STATE_IDLE_INDEX;
            evt_trigger = false;
        }else {
            player.curr_state = PLAYER_STATE_MOVING;
            player.frame_index = PLAYER_STATE_MOVING_INDEX;
            player.orientation = pl->speed > 0;
        }
        evt_trigger = false;
    }
    //pl->x += (pl->x * updater_get_delta_time())/pl->speed;
}

void player_render_cb(void* player_ins, SDL_Renderer** renderer, SDL_Texture* texture) {
    auto* pl = static_cast<Player_Typedef *>(player_ins);

    SDL_FRect player_rect = {pl->dimensions.x, pl->dimensions.y, pl->dimensions.w, pl->dimensions.h};
    SDL_Rect player_frame_rect = {pl->frame_index * PLAYER_SPRITE_FRAME_WIDTH, 0, PLAYER_SPRITE_FRAME_WIDTH, PLAYER_SPRITE_FRAME_HEIGHT};

    if(!player.orientation) {
        SDL_RenderCopyF(*renderer, texture, &player_frame_rect, &player_rect);
    }else {
        SDL_RenderCopyExF(*renderer, texture, &player_frame_rect, &player_rect, 0, nullptr, SDL_FLIP_HORIZONTAL);
    }

}

void player_animations_statemachine(void* state) {

    if(player.curr_state == PLAYER_STATE_MOVING) {
        if(player.frame_counter < 7) {
            player.frame_counter++;
        }else {
            player.frame_counter = 0;

            if(player.frame_index < PLAYER_STATE_MOVING_INDEX + PLAYER_STATE_MOVING_LENGTH - 1) {
                player.frame_index++;
            }else {
                player.frame_index = PLAYER_STATE_MOVING_INDEX;
            }
        }
    }else if(player.curr_state == PLAYER_STATE_IDLE) {
        if(player.frame_counter < 7) {
            player.frame_counter++;
        }else {
            player.frame_counter = 0;

            if(player.frame_index < PLAYER_STATE_IDLE_INDEX + PLAYER_STATE_IDLE_LENGTH - 1) {
                player.frame_index++;
            }else {
                player.frame_index = PLAYER_STATE_IDLE_INDEX;
            }
        }
    }
}

//testing
void player_click_cb(void* val) {
    log_info(PLAYER_TAG, "Player clicked");
}

void player_hover_in_cb(void* val) {
    log_info(PLAYER_TAG, "Player hover in");
}

void player_hover_out_cb(void* val) {
    log_info(PLAYER_TAG, "Player hover out");
}

G_STATUS player_set_active() {
    scenes_stage_scene("Player", 0, SCENE_MODE_CLEAR);
    return G_STATUS_OK;
}