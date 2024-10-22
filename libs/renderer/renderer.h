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

#include "app/includes.h"


//defines
#define RENDERER_TAG        "RNDR"

//methods
G_STATUS renderer_init();
G_STATUS renderer_deinit();
G_STATUS renderer_register_component(RendererComponent_Typedef item);
G_STATUS renderer_remove_component();

void renderer_create_frame();
RendererComponent_Typedef* renderer_to_instance(RendererComponent_Typedef item);

#endif