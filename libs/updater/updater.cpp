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

uint32_t u_ticks_count;
float delta_time;

UpdateComponent_Typedef* update_comp_list;

G_STATUS updater_init() {
    u_ticks_count = 0;
    delta_time = 0;
    update_comp_list = NULL;
    return G_STATUS_OK;
}

G_STATUS updater_deinit() {
    UpdateComponent_Typedef* prev = update_comp_list;
    UpdateComponent_Typedef* curr = update_comp_list->next;

    do {
        free(prev);
        prev = curr;

        if(curr != NULL) {
            curr = curr->next;
        }
    }while(curr != NULL);

    return G_STATUS_OK;
}

G_STATUS register_update_components(UpdateComponent_Typedef component) {
    if(&component == NULL) {
        return G_STATUS_FAIL;
    }

    if(update_comp_list == NULL) {
        update_comp_list = updater_component_to_instance(component);
        if(update_comp_list == NULL) {
            return G_STATUS_FAIL;
        }
    }else {
        UpdateComponent_Typedef* tmp = update_comp_list;

        while(tmp->next != NULL) {
            tmp = tmp->next;
        }

        tmp->next = updater_component_to_instance(component);
    }
    return G_STATUS_OK;
}

UpdateComponent_Typedef* updater_component_to_instance(UpdateComponent_Typedef item) {
    UpdateComponent_Typedef* tmp = (UpdateComponent_Typedef*)malloc(sizeof(UpdateComponent_Typedef));
    tmp->value = item.value;
    tmp->comp_callback = item.comp_callback;
    tmp->next = NULL;

    return tmp;    
}

G_STATUS call_updater(UpdateCallback_TypeDef *target) {
    if(target == NULL) {
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
    SysEvtItem_TypeDef *sys_evt = static_cast<SysEvtItem_TypeDef *>(get_event_list(SYS_EVENT_FLAG, 0));
    KeyEvtItem_TypeDef *key_up_evt = static_cast<KeyEvtItem_TypeDef *>(get_event_list(KEY_EVENT_FLAG, KEYUP_SUBFLAG));
    KeyEvtItem_TypeDef *key_down_evt = static_cast<KeyEvtItem_TypeDef *>(get_event_list(KEY_EVENT_FLAG, KEYDOWN_SUBFLAG));

    //call them
    status = update_sys_events(sys_evt);

    if(status == G_STATUS_FAIL) {
        return status;
    }

    status = update_key_events(key_up_evt);

    if(status == G_STATUS_FAIL) {
        return status;
    }

    status = update_key_events(key_down_evt);

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

    while(ind != NULL) {
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

G_STATUS update_key_events(KeyEvtItem_TypeDef *list) {
    G_STATUS status;
    
    KeyEvtItem_TypeDef *ind = list;

    while(ind != NULL) {
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

float updater_get_delta_time() {
    return delta_time;
}
