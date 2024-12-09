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

#include "includes.h"

G_STATUS scenes_init();
G_STATUS scenes_deinit();

G_STATUS scene_load(int scene_id);


//helpers
G_STATUS scene_clear(int scene_id);
G_STATUS scene_load_components(int scene_id);
SceneItem_TypeDef* scene_get_scene_item(int scene_id);

#endif //SCENE_H
