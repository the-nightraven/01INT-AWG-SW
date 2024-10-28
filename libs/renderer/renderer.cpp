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
RendererComponentHandler obj_handler_counter;

G_STATUS renderer_init() {
    renderer_visible_list = NULL;
    obj_handler_counter = 0;

    RenderEngine eng_check = renderer_get_engine();
    if(eng_check != RENDERER_ENGINE_SDL2) {
        return G_STATUS_FAIL;
    }

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

RendererComponentHandler renderer_register_component(RendererComponent_Typedef item) {
    RendererComponent_Typedef* tmp = renderer_to_instance(item);

    if(tmp == NULL) {
        return G_STATUS_FAIL;
    }

    tmp->handler = obj_handler_counter++;

    if(renderer_visible_list == NULL) {
        renderer_visible_list = tmp;
    }else {
        RendererComponent_Typedef* ind = renderer_visible_list;

        while(ind->next != NULL) {
            ind = ind->next;
        }

        ind->next = tmp;
    }
    return tmp->handler;
}

G_STATUS renderer_remove_component(RendererComponentHandler handler) {
    RendererComponent_Typedef* prev = renderer_visible_list;
    RendererComponent_Typedef* curr = prev->next;

    bool found = false;
    while(curr != NULL  && !found) {
        if(curr->handler == handler) {
            found = true;
            prev->next = curr->next;
            free(curr);
        }
        prev = prev->next;
        curr = prev->next;
    }
    
    if(found) {
        return G_STATUS_OK;
    }
    return G_STATUS_FAIL;
}

void renderer_create_frame(SDL_Renderer** renderer) {
    //for now render based on visibility
    RendererComponent_Typedef* ind = renderer_visible_list;

    while(ind != NULL) {
        ind->obj_render(ind->object, renderer);
        ind = ind->next;
    }
    return;
}

RendererComponent_Typedef* renderer_to_instance(RendererComponent_Typedef item) {
    RendererComponent_Typedef* tmp = (RendererComponent_Typedef*)malloc(sizeof(RendererComponent_Typedef));
    tmp->visibility = item.visibility;
    tmp->obj_type = item.obj_type;
    tmp->object = item.object;
    tmp->obj_render = item.obj_render;
    tmp->handler = -1;
    tmp->next = NULL;

    return tmp;
}