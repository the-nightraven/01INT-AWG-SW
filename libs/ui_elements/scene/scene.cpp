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

    SceneItem_TypeDef* scene_temp = scene_item_to_obj(scene);
    if (scene_temp == nullptr) {
        log_error(SCENES_TAG, "Cannot add scene", -1);
        return G_STATUS_FAIL;
    }

    SceneItem_TypeDef* ind = scenes_list;

    if(ind == nullptr) {
        scenes_list = scene_temp;
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

G_STATUS scene_load(const char* name, int scene_id) {
    if(mod_init == 0) {
        log_error(SCENES_TAG, "Module not inited", -1);
        return G_STATUS_FAIL;
    }

    if(scene_clear() == G_STATUS_FAIL) {
        log_error(SCENES_TAG, "Cannot clear scene", -1);
        return G_STATUS_FAIL;
    }

    if(scene_load_components(name, scene_id) == G_STATUS_FAIL) {
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
    if(comp_init == 0) {
        log_error(SCENES_TAG, "Component list not inited", -1);
        return nullptr;
    }

    auto* temp = static_cast<SceneComponent_TypeDef *>(malloc(sizeof(SceneComponent_TypeDef)));

    temp->continuous_update = comp.continuous_update;
    temp->evt_type = comp.evt_type;
    temp->evt_def = comp.evt_def;
    temp->update_def = comp.update_def;
    temp->rnd_component = comp.rnd_component;
    temp->next = nullptr;

    return temp;
}



SceneItem_TypeDef* scene_item_to_obj(SceneItem_TypeDef scene) {
    if (mod_init == 0) {
        log_error(SCENES_TAG, "Module not inited", -1);
        return nullptr;
    }

    auto* temp = static_cast<SceneItem_TypeDef *>(malloc(sizeof(SceneItem_TypeDef)));

    temp->name = static_cast<char *>(malloc(sizeof(char) * 50));
    strcpy(temp->name, scene.name);

    temp->scene_comp_list = scene.scene_comp_list;
    temp->type = 0;
    temp->scene_id = len++;
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

G_STATUS scene_load_components(const char* name, int scene_id) {
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

    return scene_sys_load_components(item->scene_comp_list);
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
        if(mode == 0) {
            if(strcmp(ind->name, name) == 0) {
                return ind;
            }
        }else {
            if(ind->scene_id == scene_id) {
                return ind;
            }
        }
    }
    return nullptr;
}