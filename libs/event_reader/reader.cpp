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
//@TODO: Mouse support                    DONE
//@TODO: Xbox Controller support          WIP

#include "reader.h"

#include "logger.h"
#include "updater.h"


SysEvtItem_TypeDef *sys_list;
KeyEvtItem_TypeDef *key_list;
MouseEvtItem_TypeDef *mouse_list;

G_STATUS evt_init() {
    sys_list = nullptr;
    key_list = nullptr;
    mouse_list = nullptr;
    return G_STATUS_OK;
}

//@TODO: Possible memory leak if updatecallback is allocated with malloc
G_STATUS evt_deinit() {
    //@TODO memory leak on component

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

    //key
    KeyEvtItem_TypeDef* prevk = key_list;
    KeyEvtItem_TypeDef* currk = key_list->next;

    do {
        free(prevk);
        prevk = currk;

        if(currk != nullptr) {
            currk = currk->next;
        }
    }while(currk != nullptr);

    //mouse
    MouseEvtItem_TypeDef* prevm = mouse_list;
    MouseEvtItem_TypeDef* currm = mouse_list->next;

    do {
        free(prevm);
        prevm = currm;

        if(currm != nullptr) {
            currm = currm->next;
        }
    }while(currm != nullptr);

    return G_STATUS_OK;
}

G_STATUS register_sys_event(bool essential, SysEvt_TypeDef* evt) {
    if(evt == nullptr) {
        return G_STATUS_FAIL;
    }

    if(sys_list == nullptr) {
        sys_list = static_cast<SysEvtItem_TypeDef *>(init_event_list(SYS_EVENT_FLAG, (void *) evt, essential));
        if(sys_list == nullptr) {
                return G_STATUS_FAIL;
            }
            return G_STATUS_OK;
    }

    return add_event_item(SYS_EVENT_FLAG, (void*)sys_list, (void*)evt, essential);
}

G_STATUS register_key_event(bool essential, KeyEvt_TypeDef* evt){
    if(evt == nullptr) {
        return G_STATUS_FAIL;
    }


    if(key_list == nullptr) {
        key_list = static_cast<KeyEvtItem_TypeDef *>(init_event_list(KEY_EVENT_FLAG, (void *) evt, essential));
        if(key_list == nullptr) {
            return G_STATUS_FAIL;
        }
        return G_STATUS_OK;
    }

    return add_event_item(KEY_EVENT_FLAG, (void*)key_list, (void*)evt, essential);
}

G_STATUS register_mouse_event(bool essential, MouseEvt_TypeDef* evt) {
    if(evt == nullptr) {
        return G_STATUS_FAIL;
    }

    if(mouse_list == nullptr) {
        mouse_list = static_cast<MouseEvtItem_TypeDef *>(init_event_list(MOUSE_EVENT_FLAG, (void*)evt, essential));
        if(mouse_list == nullptr) {
            return G_STATUS_FAIL;
        }
        return G_STATUS_OK;
    }

    return add_event_item(MOUSE_EVENT_FLAG, (void*)mouse_list, (void*)evt, essential);
}

G_STATUS poll_events(SDL_Event* e) {
    while(SDL_PollEvent(e)) {
        if(e->type == SDL_KEYDOWN) {
            auto *ind = static_cast<KeyEvtItem_TypeDef *>(get_event_by_hook(KEY_EVENT_FLAG, key_list, e->key.keysym.scancode));
            if(ind != nullptr) {
                if(ind->evt.kdown_update_cb.obj_callback != nullptr) {
                    if(!ind->evt.use_keyhold_protection) {
                        ind->evt.kdown_update_cb.flag = true;
                    }else {
                        if(e->key.repeat == 0) {
                            ind->evt.kdown_update_cb.flag = true;
                        }
                    }
                }
            }
        }else if(e->type == SDL_KEYUP) {
            auto *ind = static_cast<KeyEvtItem_TypeDef *>(get_event_by_hook(KEY_EVENT_FLAG, key_list, e->key.keysym.scancode));
            if(ind != nullptr) {
                if(ind->evt.kup_update_cb.obj_callback != nullptr) {
                    if(!ind->evt.use_keyhold_protection) {
                        ind->evt.kup_update_cb.flag = true;
                    }else {
                        if(e->key.repeat == 0) {
                            ind->evt.kup_update_cb.flag = true;
                        }
                    }
                }
            }
        }else if(e->type == SDL_MOUSEBUTTONDOWN) {
            int x,y;
            SDL_GetMouseState(&x,&y);
            auto *ind = static_cast<MouseEvtItem_TypeDef *>(get_event_by_mpos(x, y));

            if(ind != nullptr) {
                if(ind->evt.click_cb.obj_callback != nullptr) {
                    ind->evt.click_cb.flag = true;
                }
            }
        }else if(e->type == SDL_MOUSEMOTION) {
            int x, y;
            SDL_GetMouseState(&x,&y);
            auto *ind = mouse_list;

            while(ind != nullptr) {
                bool cond = evt_fits_in_rect(x, y, *ind->evt.dim);
                if(ind->evt.isHovering ^ cond) {
                    if(cond && ind->evt.hover_in_cb.obj_callback != nullptr) {
                        ind->evt.hover_in_cb.flag = true;
                    }else if(!cond && ind->evt.hover_out_cb.obj_callback != nullptr) {
                        ind->evt.hover_out_cb.flag = true;
                    }
                    ind->evt.isHovering = cond;
                }
                ind = ind->next;
            }

        }else {
            auto *ind = static_cast<SysEvtItem_TypeDef *>(get_event_by_hook(SYS_EVENT_FLAG, sys_list, e->type));
            if(ind != nullptr) {
                ind->evt.update_cb.flag = true;
            }
        }
    }
    return G_STATUS_OK;
}

void evt_clear_nonessential() {
    evt_clear_key_nonessential();
    evt_clear_mouse_nonessential();
    evt_clear_sys_nonessential();
    log_info(EVT_TAG, "Cleared nonessentials");
    return;
}

void evt_clear_key_nonessential() {

    KeyEvtItem_TypeDef* ind = key_list;
    if(ind == nullptr) {
#if DEBUG
        log_debug(EVT_TAG, "key list is already empty", -2);
#endif
        return;
    }

    KeyEvtItem_TypeDef* prev = nullptr;

    while(ind != nullptr) {
        if(ind->essential == ENGINE_NONESSENTIAL_COMPONENT) {
            if(prev == nullptr) {
                key_list = key_list->next;
                free(ind);
                ind = key_list;
            }else {
                prev->next = ind->next;
                free(ind);
                ind = prev->next;
            }
        }else {
            prev = ind;
            ind = ind->next;
        }
    }

    return;
}

void evt_clear_mouse_nonessential() {

    MouseEvtItem_TypeDef* ind = mouse_list;
    if(ind == nullptr) {
#if DEBUG
        log_debug(EVT_TAG, "key list is already empty", -2);
#endif
        return;
    }

    MouseEvtItem_TypeDef* prev = nullptr;

    while(ind != nullptr) {
        if(ind->essential == ENGINE_NONESSENTIAL_COMPONENT) {
            if(prev == nullptr) {
                mouse_list = mouse_list->next;
                free(ind);
                ind = mouse_list;
            }else {
                prev->next = ind->next;
                free(ind);
                ind = prev->next;
            }
        }else {
            prev = ind;
            ind = ind->next;
        }
    }

    return;
}

void evt_clear_sys_nonessential() {

    SysEvtItem_TypeDef* ind = sys_list;
    if(ind == nullptr) {
#if DEBUG
        log_debug(EVT_TAG, "key list is already empty", -2);
#endif
        return;
    }

    SysEvtItem_TypeDef* prev = nullptr;

    while(ind != nullptr) {
        if(ind->essential == ENGINE_NONESSENTIAL_COMPONENT) {
            if(prev == nullptr) {
                sys_list = sys_list->next;
                free(ind);
                ind = sys_list;
            }else {
                prev->next = ind->next;
                free(ind);
                ind = prev->next;
            }
        }else {
            prev = ind;
            ind = ind->next;
        }
    }

    return;
}

G_STATUS add_event_item(uint8_t type_flag, void *list, void *item, bool essential) {
    if(list == item) {
        return G_STATUS_FAIL;
    }

    void* tmp = init_event_list(type_flag, item, essential);

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

    }

    if(type_flag == KEY_EVENT_FLAG) {
        auto *ind = static_cast<KeyEvtItem_TypeDef *>(list);

        while(ind->next != nullptr) {
            ind = ind->next;
        }

        ind->next = static_cast<KeyEvtItem_TypeDef *>(tmp);
        return G_STATUS_OK;
    }

    if(type_flag == MOUSE_EVENT_FLAG) {
        auto *ind = static_cast<MouseEvtItem_TypeDef *>(list);

        while(ind->next != nullptr) {
            ind = ind->next;
        }

        ind->next = static_cast<MouseEvtItem_TypeDef *>(tmp);
        return G_STATUS_OK;
    }

    return G_STATUS_FAIL;
}

void* init_event_list(uint8_t type_flag, void *item, bool essential) {
    void* status;

    if(type_flag == SYS_EVENT_FLAG) {
        auto *tmp = static_cast<SysEvtItem_TypeDef *>(malloc(sizeof(SysEvtItem_TypeDef)));
        status = memcpy(&(tmp->evt), item, sizeof(SysEvt_TypeDef));

        if(status == nullptr) {
            return nullptr;
        }

        tmp->next = nullptr;
        tmp->essential = essential;
        return tmp;
    }

    if(type_flag == KEY_EVENT_FLAG) {
        auto *tmp = static_cast<KeyEvtItem_TypeDef *>(malloc(sizeof(KeyEvtItem_TypeDef)));
        status = memcpy(&(tmp->evt), item, sizeof(KeyEvt_TypeDef));

        if(status == nullptr) {
            return nullptr;
        }

        tmp->next = nullptr;
        tmp->essential = essential;
        return tmp;
    }

    if(type_flag == MOUSE_EVENT_FLAG) {
        auto *tmp = static_cast<MouseEvtItem_TypeDef *>(malloc(sizeof(MouseEvtItem_TypeDef)));
        status = memcpy(&(tmp->evt), item, sizeof(MouseEvt_TypeDef));

        if(status == nullptr) {
            return nullptr;
        }

        tmp->next = nullptr;
        tmp->essential = essential;
        return tmp;
    }

    return nullptr;
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
    }

    return nullptr;
}

void* get_event_by_mpos(int x, int y) {
    MouseEvtItem_TypeDef *ind = mouse_list;

    while(ind != nullptr) {
        if(evt_fits_in_rect(x, y, *ind->evt.dim)) {
            return ind;
        }
        ind = ind->next;
    }

    return nullptr;
}

void* get_event_list(uint8_t type_flag) {
    if(type_flag == SYS_EVENT_FLAG) {
        return (void*)sys_list;
    }

    if(type_flag == KEY_EVENT_FLAG) {
        return (void*)key_list;
    }

    if(type_flag == MOUSE_EVENT_FLAG) {
        return (void*)mouse_list;
    }

    return nullptr;
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

bool evt_fits_in_rect(int x, int y, AWG_Rect dim) {
    return x > static_cast<int>(dim.x)
        && x < static_cast<int>(dim.x + dim.w)
        && y > static_cast<int>(dim.y)
        && y < static_cast<int>(dim.y + dim.h);
}