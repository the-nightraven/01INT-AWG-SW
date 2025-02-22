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

#include <SDL_image.h>

#include "logger.h"

RendererComponent_Typedef* renderer_visible_list;
RendererComponentHandler obj_handler_counter;
SDL_Renderer** renderer_ref;

//TODO renderer_ref notnull protection

G_STATUS renderer_init(SDL_Renderer** eng_renderer) {
    if(eng_renderer == nullptr) {
        log_error(RENDERER_TAG, "Renderer doesnt exist", -1);
        return G_STATUS_FAIL;
    }
    renderer_ref = eng_renderer;

    renderer_visible_list = nullptr;
    obj_handler_counter = 0;

    RenderEngine eng_check = renderer_get_engine();
    if(eng_check != RENDERER_ENGINE_SDL2) {
        return G_STATUS_FAIL;
    }

    return G_STATUS_OK;
}

//TODO MEMORY LEAKS
G_STATUS renderer_deinit() {
    if(renderer_visible_list != nullptr) {
        RendererComponent_Typedef* prev = renderer_visible_list;
        RendererComponent_Typedef* curr = renderer_visible_list->next;

        do{
            free(prev->name);
            free(prev);
            prev = curr;

            if(curr != nullptr) {
                curr = curr->next;
            }
        }while(curr != nullptr);
    }

    return G_STATUS_OK;
}

RendererComponentHandler renderer_register_component(const RendererComponent_Typedef &item) {
    RendererComponent_Typedef* tmp = renderer_to_instance(item);
    tmp->handler = obj_handler_counter++;

    if(renderer_visible_list == nullptr) {
        renderer_visible_list = tmp;
    }else {
        RendererComponent_Typedef* ind = renderer_visible_list;

        while(ind->next != nullptr) {
            ind = ind->next;
        }

        ind->next = tmp;
    }

    log_info("RNDR", "Adding render to list");
    return tmp->handler;
}

G_STATUS renderer_remove_component(const RendererComponentHandler handler) {
    RendererComponent_Typedef* prev = nullptr;
    RendererComponent_Typedef* curr = renderer_visible_list;

    if(curr == nullptr) {
#if DEBUG
        log_debug("RNDR", "List is empty", -2);
#endif
        return G_STATUS_FAIL;
    }

    if(curr->handler == handler) {
        renderer_visible_list = renderer_visible_list->next;
        free(curr);
        return G_STATUS_OK;
    }

    while(curr != nullptr &&  curr->handler != handler) {
        prev = curr;
        curr = curr->next;
    }
    
    if(curr == nullptr) {
#if DEBUG
        log_debug("RNDR", "No handler found", -2);
#endif
        return G_STATUS_FAIL;
    }

    prev->next = curr->next;
    return G_STATUS_OK;
}

//@TODO add object free method pointer
void renderer_clear_stack() {
    if(renderer_visible_list == nullptr) {
        log_error(RENDERER_TAG, "List already empty", -1);
        return;
    }

    RendererComponent_Typedef* curr = renderer_visible_list;
    RendererComponent_Typedef* next = nullptr;

    while(curr != nullptr) {
        next = curr->next;
        free(curr->name);
        //free(curr->object);
        free(curr);
        curr = next;
    }

    renderer_visible_list = nullptr;
    obj_handler_counter = 0;
}

void renderer_create_frame(SDL_Renderer** renderer) {
    //for now render based on visibility
    RendererComponent_Typedef* ind = renderer_visible_list;

    while(ind != nullptr) {
        ind->obj_render(ind->object, renderer, ind->sprite.texture);
        ind = ind->next;
    }
}

//TODO solve memory leak
RendererComponent_Typedef* renderer_to_instance(RendererComponent_Typedef item) {
    auto* tmp = static_cast<RendererComponent_Typedef *>(malloc(sizeof(RendererComponent_Typedef)));
    tmp->visibility = item.visibility;
    tmp->name = static_cast<char*>(malloc(sizeof(char) * 50));
    strcpy(tmp->name, item.name);

    if(item.sprite.map_path != nullptr) {
        tmp->sprite.active = true;
        log_debug(RENDERER_TAG, "has sprite", 0);
        tmp->sprite.map_path = item.sprite.map_path;

        SDL_Surface* tmp_srf = IMG_Load(tmp->sprite.map_path);
        if(tmp_srf == nullptr) {
            log_error(RENDERER_TAG, "Image location error", -1);
            return nullptr;
        }
        log_debug(RENDERER_TAG, "loaded image", 0);

        tmp->sprite.texture = SDL_CreateTextureFromSurface(*renderer_ref, tmp_srf);
        if(tmp->sprite.texture == nullptr) {
            log_error(RENDERER_TAG, "Texture could not be created", -1);
            return nullptr;
        }
        log_debug(RENDERER_TAG, "created texture", 0);
        SDL_FreeSurface(tmp_srf);

    } else {
        tmp->sprite.map_path = nullptr;
        tmp->sprite.texture = nullptr;
        tmp->sprite.active = false;
    }

    tmp->animatable = item.animatable;
    tmp->animation_cb = item.animation_cb;
    tmp->obj_type = item.obj_type;
    tmp->object = item.object;
    tmp->obj_render = item.obj_render;
    tmp->handler = -1;
    tmp->next = nullptr;

    return tmp;
}

RendererComponent_Typedef* renderer_get_list() {
    return renderer_visible_list;
}

RendererComponent_Typedef* renderer_get_component(RendererComponentHandler hnd, const char* name) {
    RendererComponent_Typedef* curr = renderer_visible_list;
    int mode = 0;

    if(name == nullptr) {
        mode = 1;
    }

    while(curr != nullptr) {
        if(curr->handler == hnd && mode == 1) {
            return curr;
        }
        if(strcmp(curr->name, name) == 0 && mode == 0) {
            return curr;
        }
        curr = curr->next;
    }
    return nullptr;
}

//depricated
G_STATUS renderer_change_comp_state(RendererComponentHandler hnd, const char* name, int state) {
    RendererComponent_Typedef* curr = renderer_get_component(hnd, name);
    if(curr == nullptr) {
        log_error(RENDERER_TAG, "No such component", -1);
        return G_STATUS_FAIL;
    }
    return G_STATUS_OK;
}