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
    //delta wait

    //get events
    SysEvtItem_TypeDef *sys_evt = (SysEvtItem_TypeDef*)get_event_list(SYS_EVENT_FLAG, 0);
    KeyEvtItem_TypeDef *key_up_evt = (KeyEvtItem_TypeDef*)get_event_list(KEY_EVENT_FLAG, KEYUP_SUBFLAG);
    KeyEvtItem_TypeDef *key_down_evt = (KeyEvtItem_TypeDef*)get_event_list(KEY_EVENT_FLAG, KEYDOWN_SUBFLAG);

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
