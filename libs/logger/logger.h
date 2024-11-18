/*
###########################################################

@file: logger.h

@author: Tanasescu Darius

@Licence: Apache 2.0

@Copyleft the-nightraven, product is open source and has
absolutely no warranty, you are free to copy, distribute
and change, but not for commercial use

@Revision: InfDev

@Description: Logging system header file

###########################################################
*/

#ifndef LOGGER_HEADER
#define LOGGER_HEADER

#define INFO_MODE               0x0f
#define DEBUG_MODE              0x1f
#define ERROR_MODE              0x2f

#define INFO_TAG                "INFO"
#define DEBUG_TAG               "DEBUG"
#define ERROR_TAG               "ERROR"

#include "includes.h"

extern void system_print(char* str);

void log_info(const char* local_tag, const char* str);
void log_debug(const char* local_tag, const char* str, int value);
void log_error(const char* local_tag, const char* str, int value);

#endif //LOGGER_HEADER