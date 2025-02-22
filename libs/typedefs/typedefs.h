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

#define DEFAULT_UPDATER_CB             (UpdateCallback_TypeDef){false, nullptr, nullptr}
#define ENGINE_ESSENTIAL_COMPONENT     true
#define ENGINE_NONESSENTIAL_COMPONENT  false

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

//maybe add control stacks

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
    bool essential;
    SysEvt_TypeDef evt;
    SysEvtItem_TypeDef *next;
} SysEvtItem_TypeDef;

typedef struct KeyEvtItem_TypeDef {
    bool essential;
    KeyEvt_TypeDef evt;
    KeyEvtItem_TypeDef *next;
} KeyEvtItem_TypeDef;

typedef struct MouseEvtItem_TypeDef {
    bool essential;
    MouseEvt_TypeDef evt;
    MouseEvtItem_TypeDef *next;
} MouseEvtItem_TypeDef;

typedef struct UpdateComponent_Typedef {
    bool essential;
    void* value;
    void (*comp_callback)(void*);
    UpdateComponent_Typedef* next;
} UpdateComponent_Typedef;

//TODO (rev0.0.2) share textures
typedef struct RendererSprite_TypeDef {
    bool active;
    const char* map_path;
    SDL_Texture* texture;
} RendererSprite_TypeDef;

//@TODO add object free method pointer
typedef struct RendererComponent_Typedef {
    RendererComponentHandler handler;
    char* name;
    bool visibility;

    //sprites
    bool animatable;
    RendererSprite_TypeDef sprite;
    UpdateComponent_Typedef animation_cb;

    //object
    void* object;
    int obj_type;
    void (*obj_render)(void*, SDL_Renderer**, SDL_Texture*);

    RendererComponent_Typedef* next;
} RendererComponent_Typedef;


//scenes
//event stack
//updatestack

//TODO implement init/deinit
//TODO (maybe for optimisation) boil each event into one general type
typedef struct SceneComponent_TypeDef {
    /* staged for engine alpha 0.0.2
    int (*init_component)();
    int (*destroy_component)();
    */

    UpdateComponent_Typedef* update_def;
    KeyEvtItem_TypeDef* key_evt_def;
    MouseEvtItem_TypeDef* mouse_evt_def;
    SysEvtItem_TypeDef* sys_evt_def;
    RendererComponent_Typedef* rnd_component;
    SceneComponent_TypeDef* next;
} SceneComponent_TypeDef;

typedef struct SceneItem_TypeDef {
    int type;
    int scene_id;
    char* name;
    bool active;
    SceneComponent_TypeDef* scene_comp_list;
    SceneItem_TypeDef *next;
} SceneItem_TypeDef;


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

typedef struct ScenesModule_TypeDef {
    bool status;
    int first_scene_id; //TODO add separate ids for menu, loading and stuff
    bool change_flag;
    int change_id;
    int change_mode;
    int id_pointer;
} ScenesModule_TypeDef;

typedef struct MonitorComponents_TypeDef {
    EvtModule_TypeDef event_module;
    UpdaterModule_TypeDef updater_module;
    RendererModule_TypeDef renderer_module;
    DebugModule_TypeDef debug_module;
    WindowModule_TypeDef window_module;
    ScenesModule_TypeDef scenes_module;
    bool isRunning;
    SDL_Window* engine_display;
    SDL_Renderer* engine_renderer;
    SDL_Event engine_event_pool;
    int frameTime;
    bool fullScreen;
} MonitorComponents_TypeDef;

#endif
