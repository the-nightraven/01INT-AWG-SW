/*
###########################################################

@file: renderer.cpp

@author: Tanasescu Darius

@Licence: Apache2.0

@Copyleft the-nightraven, product is open source and has
absolutely no warranty, you are free to copy, distribute
and change, but not for commercial use

@Revision: InfDev

@Description: object rendering logic

###########################################################
*/

#include "renderer.h"

RendererComponent_Typedef* renderer_visible_list;

G_STATUS renderer_init() {
    renderer_visible_list = NULL;

    return G_STATUS_OK;
}

G_STATUS renderer_deinit() {
    RendererComponent_Typedef* prev = renderer_visible_list;
    RendererComponent_Typedef* curr = renderer_visible_list->next;

    do{
        free(prev);
        prev = curr;
        curr = curr->next;
    }while(curr != NULL);

    return G_STATUS_OK;
}
G_STATUS renderer_register_component();
G_STATUS renderer_remove_component();

void renderer_create_frame();
RendererComponent_Typedef* renderer_to_instance(RendererComponent_Typedef item);