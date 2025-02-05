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

//INCLUDES
#include "includes.h"


//DEBUG TAG
/*
  @desc Used to debug the module
*/
#define SCENES_TAG           "SCEN"


//SWITCHING MODES
/*
  @desc Puts components on top of current components
*/
#define SCENE_MODE_ON_TOP    0x00

/*
  @desc Puts components on top and clears the current scene
*/
#define SCENE_MODE_CLEAR     0x01


//EXTERNALS
/*
  @desc Clears the components from other modules
  @param (void)
  @returns G_STATUS_OK, G_STATUS_FAIL on failure
*/
extern G_STATUS scene_sys_clear_components();

/*
  @desc Loads components into other modules
  @param comp_list (SceneComponent_TypeDef) -> the component list to load
  @param mode (int) -> how to load them
  @returns G_STATUS_OK, G_STATUS_FAIL on failure
*/
extern G_STATUS scene_sys_load_components(SceneComponent_TypeDef* comp_list, int mode);


//CORE FUNCTIONS
/*
  @desc Init the module
  @param self_comp (ScenesModule_TypeDef) -> the module settings
  @returns G_STATUS_OK, G_STATUS_FAIL on failure
*/
G_STATUS scenes_init(ScenesModule_TypeDef* self_comp);

/*
  @desc Deinit the module and free memory
  @param (void)
  @returns G_STATUS_OK, G_STATUS_FAIL on failure
*/
G_STATUS scenes_deinit();

/*
  @desc High level load the scene given by name or id
  @param name (const char*) -> the name of the scene
  @param scene_id (int) -> the id of the scene
  @returns G_STATUS_OK, G_STATUS_FAIL on failure
*/
G_STATUS scenes_stage_default(const char* name, int scene_id);

/*
  @desc High level load the scene given by name or id
  @param name (const char*) -> the name of the scene
  @param scene_id (int) -> the id of the scene
  @param mode (int) -> the mode of loading
  @returns G_STATUS_OK, G_STATUS_FAIL on failure
*/
G_STATUS scenes_stage_scene(const char* name, int scene_id, int mode);

/*
  @desc High level load the scene given by name or id
  @param name (const char*) -> the name of the scene
  @param scene_id (int) -> the id of the scene
  @param mode (int) -> the mode of loading
  @returns G_STATUS_OK, G_STATUS_FAIL on failure
*/
G_STATUS scene_load(const char* name, int scene_id, int mode);

/*
  @desc High level load the scene components given by name or id
  @param name (const char*) -> the name of the scene
  @param scene_id (int) -> the id of the scene
  @param mode (int) -> the mode of loading
  @returns G_STATUS_OK, G_STATUS_FAIL on failure
*/
G_STATUS scene_load_components(const char* name, int scene_id, int mode);

/*
  @desc High level clear of the current scene
  @param (void)
  @returns G_STATUS_OK, G_STATUS_FAIL on failure
*/
G_STATUS scene_clear();


/*
  @desc High level appending of a scene
  @param scene (SceneItem_TypeDef) -> the scene item
  @returns G_STATUS_OK, G_STATUS_FAIL on failure
*/
G_STATUS scene_add(SceneItem_TypeDef scene);

/*
  @desc High level removal of the scene given by name or id
  @param name (const char*) -> the name of the scene
  @param id (int) -> the id of the scene
  @returns G_STATUS_OK, G_STATUS_FAIL on failure
*/
G_STATUS scene_remove(const char* name, int id);


//transform
/*
  @desc Low level allocation of memory for a scene component
  @param comp (SceneComponent_TypeDef) -> the object to be allocated
  @returns pointer to newly allocated memory, nullptr on failure
*/
SceneComponent_TypeDef* scene_comp_to_obj(SceneComponent_TypeDef comp);

/*
  @desc Low level allocation of memory for a scene
  @param comp (SceneItem_TypeDef) -> the object to be allocated
  @returns pointer to newly allocated memory, nullptr on failure
*/
SceneItem_TypeDef* scene_item_to_obj(SceneItem_TypeDef scene);


//getters
/*
  @desc return the object
  @param name (const char*) -> the name of the scene
  @param id (int) -> the id of the scene
  @returns pointer to allocated memory, nullptr on failure
*/
SceneItem_TypeDef* scene_get_scene_item(const char* name, int scene_id);

/*
  @desc return the object components (TODO WIP)
  @param name (const char*) -> the name of the scene
  @param id (int) -> the id of the scene
  @returns pointer to allocated memory, nullptr on failure
*/
SceneComponent_TypeDef* scene_get_comp_list();

#endif //SCENE_H
