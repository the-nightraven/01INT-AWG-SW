/*
###########################################################

@file: start_menu.cpp

@author: Tanasescu Darius

@Licence: Apache2.0

@Copyleft the-nightraven, product is open source and has
absolutely no warranty, you are free to copy, distribute
and change, but not for commercial use

@Revision: InfDev

@Description: Start menu scene logic

###########################################################
*/

#include "start_menu.h"
#include "player.h"
#include "engine_monitor.h"

SceneItem_TypeDef start_menu;
StartMenu_TypeDef start_menu_obj;


G_STATUS init_start_menu() {
    start_menu_obj.start.x = MENU_START_X;
    start_menu_obj.start.y = MENU_START_Y;
    start_menu_obj.start.w = MENU_START_W;
    start_menu_obj.start.h = MENU_START_H;
    start_menu_obj.st_hover = false;

    start_menu_obj.options.x = MENU_OPTIONS_X;
    start_menu_obj.options.y = MENU_OPTIONS_Y;
    start_menu_obj.options.w = MENU_OPTIONS_W;
    start_menu_obj.options.h = MENU_OPTIONS_H;
    start_menu_obj.op_hover = false;

    start_menu_obj.exit.x = MENU_EXIT_X;
    start_menu_obj.exit.y = MENU_EXIT_Y;
    start_menu_obj.exit.w = MENU_EXIT_W;
    start_menu_obj.exit.h = MENU_EXIT_H;
    start_menu_obj.ex_hover = false;

    return G_STATUS_OK;
}

G_STATUS deinit_start_menu() {
    return G_STATUS_OK;
}

G_STATUS register_start_menu() {

    //updater callback
    UpdateCallback_TypeDef start_press_cb = {false, &start_menu_obj, start_press};
    UpdateCallback_TypeDef start_hover_in_cb = {false, &start_menu_obj, start_hover_in};
    UpdateCallback_TypeDef start_hover_out_cb = {false, &start_menu_obj, start_hover_out};

    UpdateCallback_TypeDef options_press_cb = {false, &start_menu_obj, options_press};
    UpdateCallback_TypeDef options_hover_in_cb = {false, &start_menu_obj, options_hover_in};
    UpdateCallback_TypeDef options_hover_out_cb = {false, &start_menu_obj, options_hover_out};

    UpdateCallback_TypeDef exit_press_cb = {false, &start_menu_obj, exit_press};
    UpdateCallback_TypeDef exit_hover_in_cb = {false, &start_menu_obj, exit_hover_in};
    UpdateCallback_TypeDef exit_hover_out_cb = {false, &start_menu_obj, exit_hover_out};

    //event callback
    MouseEvt_TypeDef start_mouse_evt_cb = {&(start_menu_obj.start), false, start_hover_in_cb, start_hover_out_cb, start_press_cb};
    MouseEvt_TypeDef options_mouse_evt_cb = {&(start_menu_obj.options), false, options_hover_in_cb, options_hover_out_cb, options_press_cb};
    MouseEvt_TypeDef exit_mouse_evt_cb = {&(start_menu_obj.exit), false, exit_hover_in_cb, exit_hover_out_cb, exit_press_cb};

    MouseEvtItem_TypeDef sme = {ENGINE_NONESSENTIAL_COMPONENT, start_mouse_evt_cb, nullptr};
    MouseEvtItem_TypeDef ome = {ENGINE_NONESSENTIAL_COMPONENT, options_mouse_evt_cb, nullptr};
    MouseEvtItem_TypeDef eme = {ENGINE_NONESSENTIAL_COMPONENT, exit_mouse_evt_cb, nullptr};

    //renderer callback
    RendererSprite_TypeDef s;
    s.map_path = "../assets/ui-ux/UIMenu.png";
    RendererComponent_Typedef start_render_comp = {0, "start_button", true, s, &start_menu_obj, 0, start_button_render, nullptr};
    RendererComponent_Typedef options_render_comp = {0, "options_button", true, s, &start_menu_obj, 0, options_button_render, nullptr};
    RendererComponent_Typedef exit_render_comp = {0, "exit_button", true, s, &start_menu_obj, 0, exit_button_render, nullptr};

    //scene components
    SceneComponent_TypeDef start_comp = {nullptr, nullptr, &sme, nullptr, &start_render_comp, nullptr};
    SceneComponent_TypeDef options_comp = {nullptr, nullptr, &ome, nullptr, &options_render_comp, nullptr};
    SceneComponent_TypeDef exit_comp = {nullptr, nullptr, &eme, nullptr, &exit_render_comp, nullptr};

    SceneComponent_TypeDef* psm = &start_comp;
    SceneComponent_TypeDef* pom = &options_comp;
    SceneComponent_TypeDef* pem = &exit_comp;

    psm->next = pom;
    pom->next = pem;
    pem->next = nullptr;

    //TODO address memory location escape
    start_menu.name = "start_menu";
    start_menu.scene_comp_list = psm;
    start_menu.type = 0;
    start_menu.active = false;
    start_menu.next = nullptr;

    scene_add(start_menu);

    return G_STATUS_OK;
}

G_STATUS start_menu_set_active() {
    return scenes_stage_default("start_menu", 0);
}


//callbacks
void start_press(void* obj) {
    log_info(START_MENU_TAG, "Pressed start");
    static_cast<StartMenu_TypeDef *>(obj)->st_hover = false;
    player_set_active();
}

void start_hover_in(void* obj) {
    static_cast<StartMenu_TypeDef *>(obj)->st_hover = true;
}

void start_hover_out(void* obj) {
    static_cast<StartMenu_TypeDef *>(obj)->st_hover = false;
}


void options_press(void* obj) {
    log_info(START_MENU_TAG, "Pressed options");
    static_cast<StartMenu_TypeDef *>(obj)->op_hover = false;
}

void options_hover_in(void* obj) {
    static_cast<StartMenu_TypeDef *>(obj)->op_hover = true;
}

void options_hover_out(void* obj) {
    static_cast<StartMenu_TypeDef *>(obj)->op_hover = false;
}


void exit_press(void* obj) {
    log_info(START_MENU_TAG, "Pressed exit");
    static_cast<StartMenu_TypeDef *>(obj)->ex_hover = false;
    evt_push_event(SDL_QUIT, SDL_SCANCODE_UNKNOWN);
}

void exit_hover_in(void* obj) {
    static_cast<StartMenu_TypeDef *>(obj)->ex_hover = true;
}

void exit_hover_out(void* obj) {
    static_cast<StartMenu_TypeDef *>(obj)->ex_hover = false;
}


void start_button_render(void* obj, SDL_Renderer** renderer, SDL_Texture* texture) {
    auto* sm = static_cast<StartMenu_TypeDef *>(obj);
    SDL_FRect start;
    SDL_Rect src_start;
    int ind = sm->st_hover ? MENU_START_HOVER_IN_IND : MENU_START_HOVER_OUT_IND;

    src_start.x = ind * 48;
    src_start.y = 0;
    src_start.w = 48;
    src_start.h = 16;

    start.x = sm->start.x;
    start.y = sm->start.y;
    start.w = sm->start.w;
    start.h = sm->start.h;

    SDL_RenderCopyF(*renderer, texture, &src_start, &start);
}
void options_button_render(void* obj, SDL_Renderer** renderer, SDL_Texture* texture) {
    auto* om = static_cast<StartMenu_TypeDef *>(obj);
    SDL_FRect options;
    SDL_Rect src_options;
    int ind = om->op_hover ? MENU_OPTIONS_HOVER_IN_IND : MENU_OPTIONS_HOVER_OUT_IND;

    src_options.x = ind * 48;
    src_options.y = 0;
    src_options.w = 48;
    src_options.h = 16;

    options.x = om->options.x;
    options.y = om->options.y;
    options.w = om->options.w;
    options.h = om->options.h;

    SDL_RenderCopyF(*renderer, texture, &src_options, &options);
}

void exit_button_render(void* obj, SDL_Renderer** renderer, SDL_Texture* texture) {
    auto* om = static_cast<StartMenu_TypeDef *>(obj);
    SDL_FRect exit;
    SDL_Rect src_exit;
    int ind = om->ex_hover ? MENU_EXIT_HOVER_IN_IND : MENU_EXIT_HOVER_OUT_IND;

    src_exit.x = ind * 48;
    src_exit.y = 0;
    src_exit.w = 48;
    src_exit.h = 16;

    exit.x = om->exit.x;
    exit.y = om->exit.y;
    exit.w = om->exit.w;
    exit.h = om->exit.h;

    SDL_RenderCopyF(*renderer, texture, &src_exit, &exit);
}

