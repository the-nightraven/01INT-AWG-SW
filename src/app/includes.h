/*
###########################################################

@file: includes.h

@author: Tanasescu Darius

@Licence: Apache2.0

@Copyleft the-nightraven, product is open source and has
absolutely no warranty, you are free to copy, distribute
and change, but not for commercial use

@Revision: InfDev

@Description: file with includes

###########################################################
*/

#ifndef INCLUDES_H
#define INCLUDES_H

#include <inttypes.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <malloc.h>
#include "typedefs/typedefs.h"
#include <stdbool.h>

//typedefs
typedef uint8_t G_STATUS;

//macros
#define G_STATUS_OK          0x00
#define G_STATUS_FAIL        0x01

//config
#define WINDOW_WIDTH            640
#define WINDOW_HEIGHT           480

#endif