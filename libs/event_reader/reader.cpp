/*
###########################################################

@file: reader.cpp

@author: Tanasescu Darius

@Licence: Apache2.0

@Copyleft the-nightraven, product is open source and has
absolutely no warranty, you are free to copy, distribute
and change, but not for commercial use

@Revision: InfDev

@Description: event reader

###########################################################
*/

//@TODO: System events support            DONE
//@TODO: Keyboard support                 DONE
//@TODO: Mouse support                    WIP
//@TODO: Xbox Controller support          WIP

#include "reader.h"


SysEvtItem_TypeDef *sys_list;
KeyEvtItem_TypeDef *key_up_list;
KeyEvtItem_TypeDef *key_down_list;

G_STATUS evt_init() {
    sys_list = NULL;
    key_down_list = NULL;
    key_up_list = NULL;
    return G_STATUS_OK;
}

G_STATUS evt_deinit() {
    //sys
    SysEvtItem_TypeDef* prev = sys_list;
    SysEvtItem_TypeDef* curr = sys_list->next;

    do{
        free(prev);
        prev = curr;

        if(curr != NULL) {
            curr = curr->next;
        }
    }while(curr != NULL);
    //keyup
    KeyEvtItem_TypeDef* prevk = key_up_list;
    KeyEvtItem_TypeDef* currk = key_up_list->next;

    do {
        free(prevk);
        prevk = currk;

        if(currk != NULL) {
            currk = currk->next;
        }
    }while(currk != NULL);

    //keydown
    prevk = key_down_list;
    currk = key_down_list->next;

    do {
        free(prevk);
        prevk = currk;

        if(currk != NULL) {
            currk = currk->next;
        }
    }while(currk != NULL);

    return G_STATUS_OK;
}

G_STATUS register_sys_event(SysEvt_TypeDef* evt) {
    if(evt == NULL) {
        return G_STATUS_FAIL;
    }

    if(sys_list == NULL) {
        sys_list = (SysEvtItem_TypeDef*)init_event_list(SYS_EVENT_FLAG, (void*)evt);
        if(sys_list == NULL) {
                return G_STATUS_FAIL;
            }
            return G_STATUS_OK;
    }else {
        return add_event_item(SYS_EVENT_FLAG, (void*)sys_list, (void*)evt);
    }
}

G_STATUS register_key_event(KeyEvt_TypeDef* evt){
    if(evt == NULL) {
        return G_STATUS_FAIL;
    }

    if((*evt).SDL_Hook == SDL_KEYDOWN) {
        if(key_down_list == NULL) {
            key_down_list = (KeyEvtItem_TypeDef*)init_event_list(KEY_EVENT_FLAG, (void*)evt);
            if(key_down_list == NULL) {
                return G_STATUS_FAIL;
            }
            return G_STATUS_OK;
        }else {
            return add_event_item(KEY_EVENT_FLAG, (void*)key_down_list, (void*)evt);
        }
    }else if((*evt).SDL_Hook == SDL_KEYUP) {
        if(key_up_list == NULL) {
            key_up_list = (KeyEvtItem_TypeDef*)init_event_list(KEY_EVENT_FLAG, (void*)evt);
            if(key_up_list == NULL) {
                return G_STATUS_FAIL;
            }
            return G_STATUS_OK;
        }else {
            return add_event_item(KEY_EVENT_FLAG, (void*)key_up_list, (void*)evt);
        }
    }else {
        return G_STATUS_FAIL;
    }
}

G_STATUS poll_events(SDL_Event* e) {
    while(SDL_PollEvent(e)) {
        if((*e).type == SDL_KEYDOWN) {
            KeyEvtItem_TypeDef *ind = (KeyEvtItem_TypeDef*)get_event_by_hook(KEY_EVENT_FLAG, key_down_list, (*e).key.keysym.scancode);
            if(ind != NULL) {
                if(!ind->evt.use_keyhold_protection) {
                    ind->evt.update_cb.flag = true;
                }else {
                    if((*e).key.repeat == 0) {
                        ind->evt.update_cb.flag = true;
                    }
                }
            }
        }else if((*e).type == SDL_KEYUP) {
            KeyEvtItem_TypeDef *ind = (KeyEvtItem_TypeDef*)get_event_by_hook(KEY_EVENT_FLAG, key_up_list, (*e).key.keysym.scancode);
            if(ind != NULL) {
                if(!ind->evt.use_keyhold_protection) {
                    ind->evt.update_cb.flag = true;
                }else {
                    if((*e).key.repeat == 0) {
                        ind->evt.update_cb.flag = true;
                    }
                }
            }
        }else {
            SysEvtItem_TypeDef *ind = (SysEvtItem_TypeDef*)get_event_by_hook(SYS_EVENT_FLAG, sys_list, (*e).type);
            if(ind != NULL) {
                if(!ind->evt.use_keyhold_protection) {
                    ind->evt.update_cb.flag = true;
                }else {
                    if((*e).key.repeat == 0) {
                        ind->evt.update_cb.flag = true;
                    }
                }
            }
        }
    }
    return G_STATUS_OK;
}

G_STATUS add_event_item(uint8_t type_flag, void *list, void *item) {
    if(list == item) {
        return G_STATUS_FAIL;
    }

    void* tmp = init_event_list(type_flag, item);

    if(tmp == NULL) {
        return G_STATUS_FAIL;
    }

    if(type_flag == SYS_EVENT_FLAG) {
        SysEvtItem_TypeDef *ind = (SysEvtItem_TypeDef*)list;

        while(ind->next != NULL) {
            ind = ind->next;
        }
        
        ind->next = (SysEvtItem_TypeDef*)tmp;
        return G_STATUS_OK;

    }else if(type_flag == KEY_EVENT_FLAG) {
        KeyEvtItem_TypeDef *ind = (KeyEvtItem_TypeDef*)list;

        while(ind->next != NULL) {
            ind = ind->next;
        }

        ind->next = (KeyEvtItem_TypeDef*)tmp;
        return G_STATUS_OK;
    }else {
        return G_STATUS_FAIL;
    }
}

void* init_event_list(uint8_t type_flag, void *item) {
    void* status;
    if(type_flag == SYS_EVENT_FLAG) {
        SysEvtItem_TypeDef *tmp = (SysEvtItem_TypeDef*)malloc(sizeof(SysEvtItem_TypeDef));
        status = memcpy(&(tmp->evt), item, sizeof(SysEvt_TypeDef));

        if(status == NULL) {
            return NULL;
        }

        tmp->next = NULL;
        return tmp;
    }else if(type_flag == KEY_EVENT_FLAG) {
        KeyEvtItem_TypeDef *tmp = (KeyEvtItem_TypeDef*)malloc(sizeof(KeyEvtItem_TypeDef));
        status = memcpy(&(tmp->evt), item, sizeof(KeyEvt_TypeDef));

        if(status == NULL) {
            return NULL;
        }

        tmp->next = NULL;
        return tmp;
    }else {
        return NULL;
    }
}

void* get_event_by_hook(uint8_t type_flag, void *list, int hook) {
    if(list == NULL) {
        return NULL;
    }

    if(type_flag == SYS_EVENT_FLAG) {
        SysEvtItem_TypeDef *ind = (SysEvtItem_TypeDef*)list;

        while(ind != NULL) {
            if(ind->evt.SDL_Hook == hook) {
                return ind;
            }
            ind = ind->next;
        }
        return NULL;
    }else if(type_flag == KEY_EVENT_FLAG) {
        KeyEvtItem_TypeDef *ind = (KeyEvtItem_TypeDef*)list;

        while(ind != NULL) {
            if(ind->evt.Key == hook) {
                return ind;
            }
            ind = ind->next;
        }
        return NULL;
    }else {
        return NULL;
    }
}

void* get_event_list(uint8_t type_flag, uint8_t subflag) {
    if(type_flag == SYS_EVENT_FLAG) {
        return (void*)sys_list;
    }else if(type_flag == KEY_EVENT_FLAG) {
        if(subflag == KEYDOWN_SUBFLAG) {
            return (void*)key_down_list;
        }else if(subflag == KEYUP_SUBFLAG) {
            return (void*)key_up_list;
        }else {
            return NULL;
        }
    }else {
        return NULL;
    }
}