/*
###########################################################

@file:scene.h

@author: Tanasescu Darius

@Licence: Apache2.0

@Copyleft the-nightraven, product is open source and has
absolutely no warranty, you are free to copy, distribute
and change, but not for commercial use

@Revision: InfDev

@Description: source file with scene functionality

###########################################################
*/

#include "scene.h"

#include <iterator>

#include "logger.h"

SceneItem_TypeDef* scenes_list = nullptr;
SceneComponent_TypeDef* temp_comp_list = nullptr;
int len;
int mod_init = 0;
int comp_init = 0;

G_STATUS scenes_init() {
    if (mod_init != 0) {
        log_error(SCENES_TAG, "Module already inited", -1);
        return G_STATUS_FAIL;
    }
    //scenes_list = static_cast<SceneItem_TypeDef *>(malloc(sizeof(SceneItem_TypeDef)));
    len = 0;
    mod_init = 1;
    log_info(SCENES_TAG, "Inited module");
    return G_STATUS_OK;
}

G_STATUS scenes_deinit() {
    if(mod_init == 0) {
        log_error(SCENES_TAG, "Module not inited", -1);
        return G_STATUS_FAIL;
    }
    //free list
    SceneItem_TypeDef* head = scenes_list;
    SceneItem_TypeDef* curr;

    while(head != nullptr) {
        curr = head;
        head = curr->next;
        free(curr->name);
        //comp free workaround
        temp_comp_list = curr->scene_comp_list;
        scene_deinit_comp_list();
        free(curr);
    }

    len = 0;
    mod_init = 0;
    log_info(SCENES_TAG, "Deinited module");
    return G_STATUS_OK;
}

G_STATUS scene_add(SceneItem_TypeDef scene) {
    if (mod_init == 0) {
        log_error(SCENES_TAG, "Module not inited", -1);
        return G_STATUS_FAIL;
    }

    printf("HERE");
    SceneItem_TypeDef* scene_temp = scene_item_to_obj(scene);
    printf("HERE");
    if (scene_temp == nullptr) {
        log_error(SCENES_TAG, "Cannot add scene", -1);
        return G_STATUS_FAIL;
    }

    SceneItem_TypeDef* ind = scenes_list;

    if(ind == nullptr) {
        scenes_list = scene_temp;
        log_info(SCENES_TAG, "Added scene");
        return G_STATUS_OK;
    }

    while(ind->next != nullptr) {
        ind = ind->next;
    }

    ind->next = scene_temp;
    log_info(SCENES_TAG, "Added scene");
    return G_STATUS_OK;
}

G_STATUS scene_remove(const char* name, int id) {
    //remove scene
    return G_STATUS_OK;
}

G_STATUS scene_load(const char* name, int scene_id, int mode) {
    if(mod_init == 0) {
        log_error(SCENES_TAG, "Module not inited", -1);
        return G_STATUS_FAIL;
    }

    if(scene_load_components(name, scene_id, mode) == G_STATUS_FAIL) {
        log_error(SCENES_TAG, "Cannot load scene", -1);
        return G_STATUS_FAIL;
    }
    log_info(SCENES_TAG, "Loaded scene");

    return G_STATUS_OK;
}


//helpers
G_STATUS scene_init_comp_list() {
    if(comp_init != 0) {
        log_error(SCENES_TAG, "Component list already inited", -1);
        return G_STATUS_FAIL;
    }

    comp_init = 1;
    log_info(SCENES_TAG, "Inited component list");
    return G_STATUS_OK;
}

G_STATUS scene_deinit_comp_list() {
    if(comp_init == 0) {
        log_error(SCENES_TAG, "Component list already deinited", -1);
        return G_STATUS_FAIL;
    }


    SceneComponent_TypeDef* head = temp_comp_list;
    SceneComponent_TypeDef* curr;

    while(head != nullptr) {
        curr = head;
        head = head->next;
        free(curr);
    }

    comp_init = 0;
    log_info(SCENES_TAG, "Deinited component list");
    return G_STATUS_OK;
}

G_STATUS scene_add_comp(SceneComponent_TypeDef comp) {
    if(comp_init == 0) {
        log_error(SCENES_TAG, "Component list not inited", -1);
        return G_STATUS_FAIL;
    }

    auto* comp_temp = scene_comp_to_obj(comp);
    if(comp_temp == nullptr) {
        log_error(SCENES_TAG, "Cannot add scene", -1);
        return G_STATUS_FAIL;
    }

    if(temp_comp_list == nullptr) {
        temp_comp_list = comp_temp;
        return G_STATUS_OK;
    }

    SceneComponent_TypeDef* ind = temp_comp_list;
    while(ind->next != nullptr) {
        ind = ind->next;
    }
    ind->next = comp_temp;
    log_info(SCENES_TAG, "Added scene component");
    return G_STATUS_OK;

}

SceneComponent_TypeDef* scene_get_comp_list() {
    if(comp_init == 0) {
        log_error(SCENES_TAG, "Component list not inited", -1);
        return nullptr;
    }
    return temp_comp_list;
}

SceneComponent_TypeDef* scene_comp_to_obj(SceneComponent_TypeDef comp) {
    // if(comp_init == 0) {
    //     log_error(SCENES_TAG, "Component list not inited", -1);
    //     return nullptr;
    // }

    auto* temp = static_cast<SceneComponent_TypeDef *>(malloc(sizeof(SceneComponent_TypeDef)));
    temp->key_evt_def = nullptr;
    temp->mouse_evt_def = nullptr;
    temp->sys_evt_def = nullptr;
    temp->update_def = nullptr;
    temp->rnd_component = nullptr;
    temp->next = nullptr;

    //key events
    KeyEvtItem_TypeDef* keind = comp.key_evt_def;
    if(keind == nullptr) {
        //log_debug(SCENES_TAG, "key is null", -2);
        temp->key_evt_def = nullptr;
    }else {
        while(keind != nullptr) {
            auto* tmp = static_cast<KeyEvtItem_TypeDef *>(malloc(sizeof(KeyEvtItem_TypeDef)));
            memcpy(tmp, keind, sizeof(KeyEvtItem_TypeDef));
            tmp->next = nullptr;

            if(temp->key_evt_def == nullptr) {
                temp->key_evt_def = tmp;
            }else {
                KeyEvtItem_TypeDef* lockind = temp->key_evt_def;
                while(lockind->next != nullptr) {
                    lockind = lockind->next;
                }
                lockind->next = tmp;
            }
            keind = keind->next;
        }
    }

    //mouse
    MouseEvtItem_TypeDef* meind = comp.mouse_evt_def;
    if(meind == nullptr) {
        //log_debug(SCENES_TAG, "mouse is null", -2);
        temp->mouse_evt_def = nullptr;
    }else {
        while(meind != nullptr) {
            printf("DADADAD");
            auto* tmp = static_cast<MouseEvtItem_TypeDef *>(malloc(sizeof(MouseEvtItem_TypeDef)));
            memcpy(tmp, meind, sizeof(MouseEvtItem_TypeDef));
            tmp->next = nullptr;

            if(temp->mouse_evt_def == nullptr) {
                temp->mouse_evt_def = tmp;
            }else {
                MouseEvtItem_TypeDef* locmind = temp->mouse_evt_def;
                while(locmind->next != nullptr) {
                    locmind = locmind->next;
                }
                locmind->next = tmp;
            }
            meind = meind->next;
        }
    }

    //system
    SysEvtItem_TypeDef* seind = comp.sys_evt_def;
    if(seind == nullptr) {
        //log_debug(SCENES_TAG, "sys is null", -2);
        temp->sys_evt_def = nullptr;
    }else {
        while(seind != nullptr) {
            auto* tmp = static_cast<SysEvtItem_TypeDef *>(malloc(sizeof(SysEvtItem_TypeDef)));
            memcpy(tmp, seind, sizeof(SysEvtItem_TypeDef));
            tmp->next = nullptr;

            if(temp->sys_evt_def == nullptr) {
                temp->sys_evt_def = tmp;
            }else {
                SysEvtItem_TypeDef* locsind = temp->sys_evt_def;
                while(locsind->next != nullptr) {
                    locsind = locsind->next;
                }
                locsind->next = tmp;
            }
            seind = seind->next;
        }
    }

    //update
    UpdateComponent_Typedef* ucind = comp.update_def;
    if(ucind == nullptr) {
        //log_debug(SCENES_TAG, "upd is null", -2);
        temp->update_def = nullptr;
    }else {
        while(ucind != nullptr) {
            auto* tmp = static_cast<UpdateComponent_Typedef *>(malloc(sizeof(UpdateComponent_Typedef)));
            memcpy(tmp, ucind, sizeof(UpdateComponent_Typedef));
            tmp->next = nullptr;

            if(temp->update_def == nullptr) {
                temp->update_def = tmp;
            }else {
                UpdateComponent_Typedef* locuind = temp->update_def;
                while(locuind->next != nullptr) {
                    locuind = locuind->next;
                }
                locuind->next = tmp;
            }
            ucind = ucind->next;
        }
    }

    if(comp.rnd_component != nullptr) {
        auto* rndtmp = static_cast<RendererComponent_Typedef*>(malloc(sizeof(RendererComponent_Typedef)));
        memcpy(rndtmp, comp.rnd_component, sizeof(RendererComponent_Typedef));
        temp->rnd_component = rndtmp;
    }else {
        //log_debug(SCENES_TAG, "rnd is null", -2);
        temp->rnd_component = nullptr;
    }

    temp->next = nullptr;

    return temp;
}



SceneItem_TypeDef* scene_item_to_obj(SceneItem_TypeDef scene) {
    if (mod_init == 0) {
        log_error(SCENES_TAG, "Module not inited", -1);
        return nullptr;
    }

    auto* temp = static_cast<SceneItem_TypeDef *>(malloc(sizeof(SceneItem_TypeDef)));
    temp->scene_comp_list = nullptr;

    temp->name = static_cast<char *>(malloc(sizeof(char) * 50));
    strcpy(temp->name, scene.name);

    SceneComponent_TypeDef* scind = scene.scene_comp_list;
    while(scind != nullptr) {
        auto* tmp = scene_comp_to_obj(*scind);
        tmp->next = nullptr;
        //add to real list
        if(temp->scene_comp_list == nullptr) {
            temp->scene_comp_list = tmp;
        }else {
            SceneComponent_TypeDef* locind = temp->scene_comp_list;
            while(locind->next != nullptr) {
                locind = locind->next;
            }
            locind->next = tmp;
        }

        scind = scind->next;
    }

    //temp->scene_comp_list = scene.scene_comp_list;
    temp->type = 0;
    temp->scene_id = len++;
    temp->active = false;
    temp->next = nullptr;

    return temp;
}

G_STATUS scene_clear() {
    if(mod_init == 0) {
        log_error(SCENES_TAG, "Module not inited", -1);
        return G_STATUS_FAIL;
    }
    //clear evt, upd, rnd components via extern function
    return scene_sys_clear_components();
}

G_STATUS scene_load_components(const char* name, int scene_id, int mode) {
    if(mod_init == 0) {
        log_error(SCENES_TAG, "Module not inited", -1);
        return G_STATUS_FAIL;
    }

    //load evt, updt, rnd components via extern funtion

    SceneItem_TypeDef* item = scene_get_scene_item(name, scene_id);
    if(item == nullptr) {
        log_error(SCENES_TAG, "Cannot find scene", -1);
        return G_STATUS_FAIL;
    }
    printf("LOADING SCENE: %s\n", item->name);

    G_STATUS status = scene_sys_load_components(item->scene_comp_list, mode);
    if(status != G_STATUS_OK) {
        log_error(SCENES_TAG, "Cannot load scene", -1);
        return G_STATUS_FAIL;
    }
    log_info(SCENES_TAG, "Loaded scene");

    SceneItem_TypeDef* ind = scenes_list;
    while(ind != nullptr) {
        ind->active = false;
        ind = ind->next;
    }

    item->active = true;
    return G_STATUS_OK;
}

SceneItem_TypeDef* scene_get_scene_item(const char* name, int scene_id) {
    if(mod_init == 0) {
        log_error(SCENES_TAG, "Module not inited", -1);
        return nullptr;
    }
    int mode = 0;

    if(name == nullptr) {
        mode = 1;
    }

    SceneItem_TypeDef* ind = scenes_list;
    while(ind != nullptr) {
        printf("AAAAPR: %s\n", ind->name);
        if(mode == 0) {
            if(strcmp(ind->name, name) == 0) {
                return ind;
            }
        }else {
            if(ind->scene_id == scene_id) {
                return ind;
            }
        }
        ind = ind->next;
    }
    return nullptr;
}