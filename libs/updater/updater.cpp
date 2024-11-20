/*
###########################################################

@file: updater.cpp

@author: Tanasescu Darius

@Licence: Apache2.0

@Copyleft the-nightraven, product is open source and has
absolutely no warranty, you are free to copy, distribute
and change, but not for commercial use

@Revision: InfDev

@Description: updater logic functionality for game loop

###########################################################
*/

#include "updater.h"
#include "reader.h"

uint32_t u_ticks_count;
float delta_time;

UpdateComponent_Typedef* update_comp_list;

G_STATUS updater_init() {
    u_ticks_count = 0;
    delta_time = 0;
    update_comp_list = nullptr;
    return G_STATUS_OK;
}

G_STATUS updater_deinit() {
    UpdateComponent_Typedef* prev = update_comp_list;
    UpdateComponent_Typedef* curr = update_comp_list->next;

    do {
        free(prev);
        prev = curr;

        if(curr != nullptr) {
            curr = curr->next;
        }
    }while(curr != nullptr);

    return G_STATUS_OK;
}

G_STATUS register_update_components(UpdateComponent_Typedef component) {

    if(update_comp_list == nullptr) {
        update_comp_list = updater_component_to_instance(component);
    }else {
        UpdateComponent_Typedef* tmp = update_comp_list;

        while(tmp->next != nullptr) {
            tmp = tmp->next;
        }

        tmp->next = updater_component_to_instance(component);
    }
    return G_STATUS_OK;
}

UpdateComponent_Typedef* updater_component_to_instance(UpdateComponent_Typedef item) {
    auto* tmp = static_cast<UpdateComponent_Typedef *>(malloc(sizeof(UpdateComponent_Typedef)));
    tmp->value = item.value;
    tmp->comp_callback = item.comp_callback;
    tmp->next = nullptr;

    return tmp;    
}

G_STATUS call_updater(UpdateCallback_TypeDef *target) {
    if(target == nullptr) {
        return G_STATUS_FAIL;
    }

    target->obj_callback(target->value);
    target->flag = false;
    return G_STATUS_OK;
}



G_STATUS updater_run_time_delta() {
    G_STATUS status;

    if(u_ticks_count != 0) {
        while (!SDL_TICKS_PASSED(SDL_GetTicks(), u_ticks_count + 16));
    }

    delta_time = (SDL_GetTicks() - u_ticks_count) / 1000.0f;
    u_ticks_count = SDL_GetTicks();
    //delta wait

    if (delta_time > 0.05f) {
      delta_time = 0.05f;
    }

    //get events
    auto *sys_evt = static_cast<SysEvtItem_TypeDef *>(get_event_list(SYS_EVENT_FLAG));
    auto *key_evt = static_cast<KeyEvtItem_TypeDef *>(get_event_list(KEY_EVENT_FLAG));
    auto *mouse_evt = static_cast<MouseEvtItem_TypeDef *>(get_event_list(MOUSE_EVENT_FLAG));

    //call them
    status = update_sys_events(sys_evt);

    if(status == G_STATUS_FAIL) {
        return status;
    }

    status = update_key_events(key_evt);

    if(status == G_STATUS_FAIL) {
        return status;
    }

    status = update_mouse_events(mouse_evt);

    if(status == G_STATUS_FAIL) {
        return status;
    }

    //get component updates
    UpdateComponent_Typedef* tmp = update_comp_list;

    //call them
    while(tmp != nullptr) {
        tmp->comp_callback(tmp->value);
        tmp = tmp->next;
    }

    return G_STATUS_OK;
}

bool check_updater_flag(UpdateCallback_TypeDef target) {
    return target.flag;
}

G_STATUS update_sys_events(SysEvtItem_TypeDef *list) {
    G_STATUS status;

    SysEvtItem_TypeDef *ind = list;

    while(ind != nullptr) {
        if(check_updater_flag(ind->evt.update_cb)) {
            status = call_updater(&ind->evt.update_cb);

            if(status == G_STATUS_FAIL) {
                return status;
            }
        }
        ind = ind->next;
    }
    return G_STATUS_OK;
}

//@TODO: may cause segfault because some functions are actually nullptr
G_STATUS update_key_events(KeyEvtItem_TypeDef *list) {
    G_STATUS status;
    
    KeyEvtItem_TypeDef *ind = list;

    while(ind != nullptr) {
        //kup
        if(check_updater_flag(ind->evt.kup_update_cb)) {
            status = call_updater(&ind->evt.kup_update_cb);
            if(status == G_STATUS_FAIL) {
                return status;
            }
        }

        //kdown
        if(check_updater_flag(ind->evt.kdown_update_cb)) {
            status = call_updater(&ind->evt.kdown_update_cb);
            if(status == G_STATUS_FAIL) {
                return status;
            }
        }
        ind = ind->next;
    }
    return G_STATUS_OK;
}

G_STATUS update_mouse_events(MouseEvtItem_TypeDef *list) {
    G_STATUS status;

    MouseEvtItem_TypeDef *ind = list;

    while(ind != nullptr) {
        //click
        if(check_updater_flag(ind->evt.click_cb)) {
            status = call_updater(&ind->evt.click_cb);
            if(status == G_STATUS_FAIL) {
                return status;
            }
        }

        //hover
        if(check_updater_flag(ind->evt.hover_in_cb)) {
            status = call_updater(&ind->evt.hover_in_cb);
            if(status == G_STATUS_FAIL) {
                return status;
            }
        }

        //unhover
        if(check_updater_flag(ind->evt.hover_out_cb)) {
            status = call_updater(&ind->evt.hover_out_cb);
            if(status == G_STATUS_FAIL) {
                return status;
            }
        }
        ind = ind->next;
    }
    return G_STATUS_OK;
}

float updater_get_delta_time() {
    return delta_time;
}
