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
    sys_list = nullptr;
    key_down_list = nullptr;
    key_up_list = nullptr;
    return G_STATUS_OK;
}

G_STATUS evt_deinit() {
    //sys
    SysEvtItem_TypeDef* prev = sys_list;
    SysEvtItem_TypeDef* curr = sys_list->next;

    do{
        free(prev);
        prev = curr;

        if(curr != nullptr) {
            curr = curr->next;
        }
    }while(curr != nullptr);
    //keyup
    KeyEvtItem_TypeDef* prevk = key_up_list;
    KeyEvtItem_TypeDef* currk = key_up_list->next;

    do {
        free(prevk);
        prevk = currk;

        if(currk != nullptr) {
            currk = currk->next;
        }
    }while(currk != nullptr);

    //keydown
    prevk = key_down_list;
    currk = key_down_list->next;

    do {
        free(prevk);
        prevk = currk;

        if(currk != nullptr) {
            currk = currk->next;
        }
    }while(currk != nullptr);

    return G_STATUS_OK;
}

G_STATUS register_sys_event(SysEvt_TypeDef* evt) {
    if(evt == nullptr) {
        return G_STATUS_FAIL;
    }

    if(sys_list == nullptr) {
        sys_list = static_cast<SysEvtItem_TypeDef *>(init_event_list(SYS_EVENT_FLAG, (void *) evt));
        if(sys_list == nullptr) {
                return G_STATUS_FAIL;
            }
            return G_STATUS_OK;
    }else {
        return add_event_item(SYS_EVENT_FLAG, (void*)sys_list, (void*)evt);
    }
}

G_STATUS register_key_event(KeyEvt_TypeDef* evt){
    if(evt == nullptr) {
        return G_STATUS_FAIL;
    }

    if(evt->SDL_Hook == SDL_KEYDOWN) {
        if(key_down_list == nullptr) {
            key_down_list = static_cast<KeyEvtItem_TypeDef *>(init_event_list(KEY_EVENT_FLAG, (void *) evt));
            if(key_down_list == nullptr) {
                return G_STATUS_FAIL;
            }
            return G_STATUS_OK;
        }else {
            return add_event_item(KEY_EVENT_FLAG, (void*)key_down_list, (void*)evt);
        }
    }else if(evt->SDL_Hook == SDL_KEYUP) {
        if(key_up_list == nullptr) {
            key_up_list = static_cast<KeyEvtItem_TypeDef *>(init_event_list(KEY_EVENT_FLAG, (void *) evt));
            if(key_up_list == nullptr) {
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
        if(e->type == SDL_KEYDOWN) {
            auto *ind = static_cast<KeyEvtItem_TypeDef *>(get_event_by_hook(KEY_EVENT_FLAG, key_down_list, e->key.keysym.scancode));
            if(ind != nullptr) {
                if(!ind->evt.use_keyhold_protection) {
                    ind->evt.update_cb.flag = true;
                }else {
                    if(e->key.repeat == 0) {
                        ind->evt.update_cb.flag = true;
                    }
                }
            }
        }else if(e->type == SDL_KEYUP) {
            auto *ind = static_cast<KeyEvtItem_TypeDef *>(get_event_by_hook(KEY_EVENT_FLAG, key_up_list, e->key.keysym.scancode));
            if(ind != nullptr) {
                if(!ind->evt.use_keyhold_protection) {
                    ind->evt.update_cb.flag = true;
                }else {
                    if(e->key.repeat == 0) {
                        ind->evt.update_cb.flag = true;
                    }
                }
            }
        }else {
            auto *ind = static_cast<SysEvtItem_TypeDef *>(get_event_by_hook(SYS_EVENT_FLAG, sys_list, e->type));
            if(ind != nullptr) {
                if(!ind->evt.use_keyhold_protection) {
                    ind->evt.update_cb.flag = true;
                }else {
                    if(e->key.repeat == 0) {
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

    if(tmp == nullptr) {
        return G_STATUS_FAIL;
    }

    if(type_flag == SYS_EVENT_FLAG) {
        auto *ind = static_cast<SysEvtItem_TypeDef *>(list);

        while(ind->next != nullptr) {
            ind = ind->next;
        }
        
        ind->next = static_cast<SysEvtItem_TypeDef *>(tmp);
        return G_STATUS_OK;

    }else if(type_flag == KEY_EVENT_FLAG) {
        auto *ind = static_cast<KeyEvtItem_TypeDef *>(list);

        while(ind->next != nullptr) {
            ind = ind->next;
        }

        ind->next = static_cast<KeyEvtItem_TypeDef *>(tmp);
        return G_STATUS_OK;
    }else {
        return G_STATUS_FAIL;
    }
}

void* init_event_list(uint8_t type_flag, void *item) {
    void* status;
    if(type_flag == SYS_EVENT_FLAG) {
        auto *tmp = static_cast<SysEvtItem_TypeDef *>(malloc(sizeof(SysEvtItem_TypeDef)));
        status = memcpy(&(tmp->evt), item, sizeof(SysEvt_TypeDef));

        if(status == nullptr) {
            return nullptr;
        }

        tmp->next = nullptr;
        return tmp;
    }else if(type_flag == KEY_EVENT_FLAG) {
        auto *tmp = static_cast<KeyEvtItem_TypeDef *>(malloc(sizeof(KeyEvtItem_TypeDef)));
        status = memcpy(&(tmp->evt), item, sizeof(KeyEvt_TypeDef));

        if(status == nullptr) {
            return nullptr;
        }

        tmp->next = nullptr;
        return tmp;
    }else {
        return nullptr;
    }
}

void* get_event_by_hook(uint8_t type_flag, void *list, int hook) {
    if(list == nullptr) {
        return nullptr;
    }

    if(type_flag == SYS_EVENT_FLAG) {
        auto *ind = static_cast<SysEvtItem_TypeDef *>(list);

        while(ind != nullptr) {
            if(ind->evt.SDL_Hook == hook) {
                return ind;
            }
            ind = ind->next;
        }
        return nullptr;
    }
    if(type_flag == KEY_EVENT_FLAG) {
        auto *ind = static_cast<KeyEvtItem_TypeDef *>(list);

        while(ind != nullptr) {
            if(ind->evt.Key == hook) {
                return ind;
            }
            ind = ind->next;
        }
        return nullptr;
    }else {
        return nullptr;
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
            return nullptr;
        }
    }else {
        return nullptr;
    }
}

G_STATUS evt_push_event(int sdl_hook, SDL_Scancode sdl_key) {
    SDL_Event push_evt;
    push_evt.type = sdl_hook;
    push_evt.key.keysym.scancode = sdl_key;
    push_evt.key.keysym.mod = KMOD_NONE;
    push_evt.key.repeat = 0;

    if(SDL_PushEvent(&push_evt) != 0) {
        return G_STATUS_FAIL;
    }
    return G_STATUS_OK;
}