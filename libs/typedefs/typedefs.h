/*
###########################################################

@file: typedefs.h

@author: Tanasescu Darius

@Licence: Apache2.0

@Copyleft the-nightraven, product is open source and has
absolutely no warranty, you are free to copy, distribute
and change, but not for commercial use

@Revision: InfDev

@Description: file with typedefs of various elements

###########################################################
*/

#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include "includes.h"

#define DEFAULT_UPDATER_CB         (UpdateCallback_TypeDef){false, nullptr, nullptr}

//data types
typedef int RendererComponentHandler;
typedef int RenderEngine;

//objects

typedef struct AWG_Rect {
    float x;
    float y;
    float w;
    float h;
} AWG_Rect;

typedef struct UpdateCallback_TypeDef{
    bool flag;
    void* value;
    void (*obj_callback)(void*);
} UpdateCallback_TypeDef;

typedef struct SysEvt_TypeDef {
    int SDL_Hook;
    UpdateCallback_TypeDef update_cb;
} SysEvt_TypeDef;

typedef struct KeyEvt_TypeDef {
    SDL_Scancode Key;
    UpdateCallback_TypeDef kdown_update_cb;
    UpdateCallback_TypeDef kup_update_cb;
    bool use_keyhold_protection;
} KeyEvt_TypeDef;

typedef struct MouseEvt_TypeDef {
    AWG_Rect* dim;
    bool isHovering;
    UpdateCallback_TypeDef hover_in_cb;
    UpdateCallback_TypeDef hover_out_cb;
    UpdateCallback_TypeDef click_cb;
} MouseEvt_TypeDef;

typedef struct SysEvtItem_TypeDef {
    SysEvt_TypeDef evt;
    SysEvtItem_TypeDef *next;
} SysEvtItem_TypeDef;

typedef struct KeyEvtItem_TypeDef {
    KeyEvt_TypeDef evt;
    KeyEvtItem_TypeDef *next;
} KeyEvtItem_TypeDef;

typedef struct MouseEvtItem_TypeDef {
    MouseEvt_TypeDef evt;
    MouseEvtItem_TypeDef *next;
} MouseEvtItem_TypeDef;

typedef struct UpdateComponent_Typedef {
    void* value;
    void (*comp_callback)(void*);
    UpdateComponent_Typedef* next;
} UpdateComponent_Typedef;

typedef struct RendererComponent_Typedef {
    RendererComponentHandler handler;
    char* name;
    bool visibility;
    void* object;
    int obj_type;
    void (*obj_render)(void*, SDL_Renderer**);
    RendererComponent_Typedef* next;
} RendererComponent_Typedef;


//engine monitor
typedef struct EvtModule_TypeDef {
    bool status;
} EvtModule_TypeDef;

typedef struct UpdaterModule_TypeDef {
    HANDLE upd_thread;
    bool th_isRunning;
    bool status;
} UpdaterModule_TypeDef;

typedef struct RendererModule_TypeDef {
    bool status;
} RendererModule_TypeDef;

typedef struct DebugModule_TypeDef {
    HANDLE dbg_thread;
    bool th_isRunning;
    bool status;
    RendererComponent_Typedef rnd_comp;
    RendererComponentHandler rnd_handler;
    int fps;
    bool mouseStackVisible;
} DebugModule_TypeDef;

typedef struct WindowModule_TypeDef {
    bool status;
} WindowModule_TypeDef;

typedef struct MonitorComponents_TypeDef {
    EvtModule_TypeDef event_module;
    UpdaterModule_TypeDef updater_module;
    RendererModule_TypeDef renderer_module;
    DebugModule_TypeDef debug_module;
    WindowModule_TypeDef window_module;
    bool isRunning;
    SDL_Window* engine_display;
    SDL_Renderer* engine_renderer;
    SDL_Event engine_event_pool;
    int frameTime;
    bool fullScreen;
} MonitorComponents_TypeDef;

#endif
