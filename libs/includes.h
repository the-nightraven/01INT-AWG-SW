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

#if defined(_WIN32) && !defined(DEBUG)
#include <windows.h>
#endif

#include <cinttypes>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>

#include "typedefs.h"

typedef uint8_t G_STATUS;

#define G_STATUS_OK          0x00
#define G_STATUS_FAIL        0x01

#define WINDOW_WIDTH            1000
#define WINDOW_HEIGHT           480

#endif