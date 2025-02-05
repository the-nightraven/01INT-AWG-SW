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
ScenesModule_TypeDef* scene_self_module = nullptr;



//CORE FUNCTIONS
/*
  @desc Init the module
  @param (void)
  @returns G_STATUS_OK, G_STATUS_FAIL on failure
*/
G_STATUS scenes_init(ScenesModule_TypeDef* self_comp) {
    if (scene_self_module != nullptr) {
        log_error(SCENES_TAG, "Module already inited", -1);
        return G_STATUS_FAIL;
    }

    scene_self_module = self_comp;
    scene_self_module->id_pointer = 0;

    log_info(SCENES_TAG, "Inited module");
    return G_STATUS_OK;
}

/*
  @desc Deinit the module and free memory
  @param (void)
  @returns G_STATUS_OK, G_STATUS_FAIL on failure
*/
G_STATUS scenes_deinit() {
    if(!scene_self_module->status) {
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
        //TODO free comp list

        free(curr);
    }

    scene_self_module->id_pointer = 0;
    scene_self_module->status = false;
    log_info(SCENES_TAG, "Deinited module");
    return G_STATUS_OK;
}


/*
  @desc High level load the scene given by name or id
  @param name (const char*) -> the name of the scene
  @param scene_id (int) -> the id of the scene
  @returns void
*/
G_STATUS scenes_stage_default(const char* name, int scene_id) {
    if(!scene_self_module->status) {
        log_error(SCENES_TAG, "Module not inited", -1);
        return G_STATUS_FAIL;
    }

    SceneItem_TypeDef* ind = scene_get_scene_item(name, scene_id);
    if(ind == nullptr) {
        log_error(SCENES_TAG, "Scene not found", -1);
        return G_STATUS_FAIL;
    }

    scene_self_module->first_scene_id = scene_id;
    scene_self_module->change_id = ind->scene_id;
    scene_self_module->change_mode = SCENE_MODE_ON_TOP;
    scene_self_module->change_flag = true;

    return G_STATUS_OK;
}

/*
  @desc High level load the scene given by name or id
  @param name (const char*) -> the name of the scene
  @param scene_id (int) -> the id of the scene
  @param mode (int) -> the mode of loading
  @returns void
*/
G_STATUS scenes_stage_scene(const char* name, int scene_id, int mode) {
    if(!scene_self_module->status) {
        log_error(SCENES_TAG, "Module not inited", -1);
        return G_STATUS_FAIL;
    }

    SceneItem_TypeDef* ind = scene_get_scene_item(name, scene_id);
    if(ind == nullptr) {
        log_error(SCENES_TAG, "Scene not found", -1);
        return G_STATUS_FAIL;
    }

    scene_self_module->change_id = ind->scene_id;
    scene_self_module->change_mode = mode;
    scene_self_module->change_flag = true;

    return G_STATUS_OK;
}

/*
  @desc High level load the scene given by name or id
  @param name (const char*) -> the name of the scene
  @param scene_id (int) -> the id of the scene
  @param mode (int) -> the mode of loading
  @returns G_STATUS_OK, G_STATUS_FAIL on failure
*/
G_STATUS scene_load(const char* name, int scene_id, int mode) {
    if(!scene_self_module->status) {
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

/*
  @desc High level load the scene components given by name or id
  @param name (const char*) -> the name of the scene
  @param scene_id (int) -> the id of the scene
  @param mode (int) -> the mode of loading
  @returns G_STATUS_OK, G_STATUS_FAIL on failure
*/
G_STATUS scene_load_components(const char* name, int scene_id, int mode) {
    if(!scene_self_module->status) {
        log_error(SCENES_TAG, "Module not inited", -1);
        return G_STATUS_FAIL;
    }

    //load evt, updt, rnd components via extern funtion

    SceneItem_TypeDef* item = scene_get_scene_item(name, scene_id);
    if(item == nullptr) {
        log_error(SCENES_TAG, "Cannot find scene", -1);
        return G_STATUS_FAIL;
    }


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

/*
  @desc High level clear of the current scene
  @param (void)
  @returns G_STATUS_OK, G_STATUS_FAIL on failure
*/
G_STATUS scene_clear() {
    if(!scene_self_module->status) {
        log_error(SCENES_TAG, "Module not inited", -1);
        return G_STATUS_FAIL;
    }
    //clear evt, upd, rnd components via extern function
    return scene_sys_clear_components();
}


/*
  @desc High level appending of a scene
  @param scene (SceneItem_TypeDef) -> the scene item
  @returns G_STATUS_OK, G_STATUS_FAIL on failure
*/
G_STATUS scene_add(SceneItem_TypeDef scene) {
    if (!scene_self_module->status) {
        log_error(SCENES_TAG, "Module not inited", -1);
        return G_STATUS_FAIL;
    }

    SceneItem_TypeDef* scene_temp = scene_item_to_obj(scene);
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

/*
  @desc High level removal of the scene given by name or id
  @param name (const char*) -> the name of the scene
  @param id (int) -> the id of the scene
  @returns G_STATUS_OK, G_STATUS_FAIL on failure
*/
G_STATUS scene_remove(const char* name, int id) {
    //remove scene
    return G_STATUS_OK;
}


//transform
/*
  @desc Low level allocation of memory for a scene component
  @param comp (SceneComponent_TypeDef) -> the object to be allocated
  @returns pointer to newly allocated memory, nullptr on failure
*/
SceneComponent_TypeDef* scene_comp_to_obj(SceneComponent_TypeDef comp) {
    if (!scene_self_module->status) {
        log_error(SCENES_TAG, "Module not inited", -1);
        return nullptr;
    }

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
#if DEBUG
        log_debug(SCENES_TAG, "key is null", -2);
#endif
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
#if DEBUG
        log_debug(SCENES_TAG, "mouse is null", -2);
#endif
        temp->mouse_evt_def = nullptr;
    }else {
        while(meind != nullptr) {
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
#if DEBUG
        log_debug(SCENES_TAG, "sys is null", -2);
#endif
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
#if DEBUG
        log_debug(SCENES_TAG, "upd is null", -2);
#endif
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
#if DEBUG
        log_debug(SCENES_TAG, "rnd is null", -2);
#endif
        temp->rnd_component = nullptr;
    }

    temp->next = nullptr;

    return temp;
}

/*
  @desc Low level allocation of memory for a scene
  @param comp (SceneItem_TypeDef) -> the object to be allocated
  @returns pointer to newly allocated memory, nullptr on failure
*/
SceneItem_TypeDef* scene_item_to_obj(SceneItem_TypeDef scene) {
    if (!scene_self_module->status) {
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

    temp->type = 0;
    temp->scene_id = scene_self_module->id_pointer;
    temp->active = false;
    temp->next = nullptr;

    scene_self_module->id_pointer++;

    return temp;
}


//getters
/*
  @desc return the object
  @param name (const char*) -> the name of the scene
  @param id (int) -> the id of the scene
  @returns pointer to allocated memory, nullptr on failure
*/
SceneItem_TypeDef* scene_get_scene_item(const char* name, int scene_id) {
    if(!scene_self_module->status) {
        log_error(SCENES_TAG, "Module not inited", -1);
        return nullptr;
    }
    int mode = 0;

    if(name == nullptr) {
        mode = 1;
    }

    SceneItem_TypeDef* ind = scenes_list;
    while(ind != nullptr) {
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

/*
  @desc return the object components (TODO WIP)
  @param name (const char*) -> the name of the scene
  @param id (int) -> the id of the scene
  @returns pointer to allocated memory, nullptr on failure
*/
SceneComponent_TypeDef* scene_get_comp_list() {
    return nullptr;
}