/*
###########################################################

@file: logger.cpp

@author: Tanasescu Darius

@Licence: Apache 2.0

@Copyleft the-nightraven, product is open source and has
absolutely no warranty, you are free to copy, distribute
and change, but not for commercial use

@Revision: InfDev

@Description: Logging system source file

###########################################################
*/

#include "logger.h"
#include <malloc.h>
#include <cstring>

void log_info(char* local_tag, char* str) {
    char* buffer = (char*)malloc(100 * sizeof(char));
    sprintf(buffer, "[%s][%s] %s", INFO_TAG, local_tag, str);
    system_print(buffer);
    free(buffer);
    return;
}


void log_debug(char* local_tag, char* str, int value) {
    char* buffer = (char*)malloc(100 * sizeof(char));
    sprintf(buffer, "[%s][%s] %s (%d)", DEBUG_TAG, local_tag, str, value);
    system_print(buffer);
    free(buffer);
    return;
}


void log_error(char* local_tag, char* str, int value) {
    char* buffer = (char*)malloc(100 * sizeof(char));
    sprintf(buffer, "[%s][%s] %s (%d)", ERROR_TAG, local_tag, str, value);
    system_print(buffer);
    free(buffer);
    return;
}

