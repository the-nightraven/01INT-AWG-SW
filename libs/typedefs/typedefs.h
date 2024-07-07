/*
###########################################################

@file: typedefs.h

@author: Tanasescu Darius

@Licence: Apache2.0

@Copyleft the-nightraven, product is open source and has
absolutely no warranty, you are free to copy, distribute
and change, but not for commercial use

@Revision: InfDev

@Description: file with typedefs of various elements

###########################################################
*/

typedef struct UpdateCallback_TypeDef{
    bool flag;
    void* value;
    void (*obj_callback)(void*);
};

typedef struct SysEvt_TypeDef {
    int SDL_Hook;
    UpdateCallback_TypeDef update_cb;
};

typedef struct KeyEvt_TypeDef {
    int SDL_Hook;
    int Key;
    UpdateCallback_TypeDef update_cb;
};

typedef struct SysEvtItem_TypeDef {
    SysEvt_TypeDef evt;
    SysEvtItem_TypeDef *next;
};

typedef struct KeyEvtItem_TypeDef {
    KeyEvt_TypeDef evt;
    KeyEvtItem_TypeDef *next;
};