/*
###########################################################

@file:scene.h

@author: Tanasescu Darius

@Licence: Apache2.0

@Copyleft the-nightraven, product is open source and has
absolutely no warranty, you are free to copy, distribute
and change, but not for commercial use

@Revision: InfDev

@Description: header file with scene functionality definitions

###########################################################
*/

#ifndef SCENE_H
#define SCENE_H

#define SCENES_TAG       "SCEN"

#include "includes.h"

#define SCENE_MODE_ON_TOP    0x00
#define SCENE_MODE_CLEAR     0x01

//externs
extern G_STATUS scene_sys_clear_components();
extern G_STATUS scene_sys_load_components(SceneComponent_TypeDef* comp_list);


//core
G_STATUS scenes_init();
G_STATUS scenes_deinit();

G_STATUS scene_load(const char* name, int scene_id, int mode);

G_STATUS scene_add(SceneItem_TypeDef scene);
G_STATUS scene_remove(const char* name, int id);


//helpers
G_STATUS scene_init_comp_list();
G_STATUS scene_deinit_comp_list();
G_STATUS scene_add_comp(SceneComponent_TypeDef comp);
SceneComponent_TypeDef* scene_get_comp_list();
SceneComponent_TypeDef* scene_comp_to_obj(SceneComponent_TypeDef comp);


SceneItem_TypeDef* scene_item_to_obj(SceneItem_TypeDef scene);
G_STATUS scene_clear();
G_STATUS scene_load_components(const char* name, int scene_id);
SceneItem_TypeDef* scene_get_scene_item(const char* name, int scene_id);

#endif //SCENE_H
