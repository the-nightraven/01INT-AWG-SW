/*
###########################################################

@file: renderer.h

@author: Tanasescu Darius

@Licence: Apache2.0

@Copyleft the-nightraven, product is open source and has
absolutely no warranty, you are free to copy, distribute
and change, but not for commercial use

@Revision: InfDev

@Description: object renderer logic header

###########################################################
*/

#ifndef RENDERER_H
#define RENDERER_H

#include "includes.h"


//defines
#define RENDERER_TAG                   "RNDR"
#define RENDERER_ENGINE_SDL2           1

//methods
G_STATUS renderer_init(SDL_Renderer** eng_renderer);
G_STATUS renderer_deinit();
RendererComponentHandler renderer_register_component(const RendererComponent_Typedef &item);
G_STATUS renderer_remove_component(RendererComponentHandler handler);

void renderer_create_frame(SDL_Renderer** renderer);
RendererComponent_Typedef* renderer_to_instance(RendererComponent_Typedef item);

void renderer_clear_stack();

//externs
extern RenderEngine renderer_get_engine();

//helpers
RendererComponent_Typedef* renderer_get_list();
RendererComponent_Typedef* renderer_get_component(RendererComponentHandler hnd, const char* name);
G_STATUS renderer_change_comp_state(RendererComponentHandler hnd, const char* name, int state);

#endif