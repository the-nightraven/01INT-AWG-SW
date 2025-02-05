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

    start_menu_obj.options.x = MENU_OPTIONS_X;
    start_menu_obj.options.y = MENU_OPTIONS_Y;
    start_menu_obj.options.w = MENU_OPTIONS_W;
    start_menu_obj.options.h = MENU_OPTIONS_H;

    start_menu_obj.exit.x = MENU_EXIT_X;
    start_menu_obj.exit.y = MENU_EXIT_Y;
    start_menu_obj.exit.w = MENU_EXIT_W;
    start_menu_obj.exit.h = MENU_EXIT_H;

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

    //done for structure of code
    MouseEvtItem_TypeDef* psme = &sme;
    MouseEvtItem_TypeDef* pome = &ome;
    MouseEvtItem_TypeDef* peme = &eme;

    psme->next = pome;
    pome->next = peme;
    peme->next = nullptr;

    //renderer callback
    RendererComponent_Typedef menu_render_comp = {0, "start_menu", true, &start_menu_obj, 0, start_menu_render, nullptr};

    //scene components
    SceneComponent_TypeDef start_comp = {nullptr, nullptr, psme, nullptr, &menu_render_comp, nullptr};

    //TODO address memory location escape
    start_menu.name = "start_menu";
    start_menu.scene_comp_list = &start_comp;
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
    player_set_active();
}

void start_hover_in(void* obj) {
    static_cast<StartMenu_TypeDef *>(obj)->start.y -= BUTTON_PADDING;
}

void start_hover_out(void* obj) {
    static_cast<StartMenu_TypeDef *>(obj)->start.y += BUTTON_PADDING;
}


void options_press(void* obj) {
    log_info(START_MENU_TAG, "Pressed options");
}

void options_hover_in(void* obj) {
    static_cast<StartMenu_TypeDef *>(obj)->options.y -= BUTTON_PADDING;
}

void options_hover_out(void* obj) {
    static_cast<StartMenu_TypeDef *>(obj)->options.y += BUTTON_PADDING;
}


void exit_press(void* obj) {
    log_info(START_MENU_TAG, "Pressed exit");
    evt_push_event(SDL_QUIT, SDL_SCANCODE_UNKNOWN);
}

void exit_hover_in(void* obj) {
    static_cast<StartMenu_TypeDef *>(obj)->exit.y -= BUTTON_PADDING;
}

void exit_hover_out(void* obj) {
    static_cast<StartMenu_TypeDef *>(obj)->exit.y += BUTTON_PADDING;
}


void start_menu_render(void* obj, SDL_Renderer** renderer) {
    auto* sm = static_cast<StartMenu_TypeDef *>(obj);
    int status;
    SDL_FRect start;
    SDL_FRect options;
    SDL_FRect exit;

    start.x = sm->start.x;
    start.y = sm->start.y;
    start.w = sm->start.w;
    start.h = sm->start.h;

    options.x = sm->options.x;
    options.y = sm->options.y;
    options.w = sm->options.w;
    options.h = sm->options.h;

    exit.x = sm->exit.x;
    exit.y = sm->exit.y;
    exit.w = sm->exit.w;
    exit.h = sm->exit.h;

    SDL_SetRenderDrawColor(*renderer, 255, 255, 255, 255);
    SDL_RenderDrawRectF(*renderer, &start);
    SDL_RenderDrawRectF(*renderer, &options);
    SDL_RenderDrawRectF(*renderer, &exit);
}
