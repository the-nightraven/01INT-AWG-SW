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
#include <cstdio>

constexpr std::size_t LOG_BUFFER_SIZE = 512;

void log_info(const char* local_tag, const char* str) {
    char buffer[LOG_BUFFER_SIZE];
    snprintf(buffer, sizeof(buffer), "[%s][%s] %s", INFO_TAG, local_tag, str);
    system_print(buffer);
}

void log_debug(const char* local_tag, const char* str, int value) {
    char buffer[LOG_BUFFER_SIZE];
    snprintf(buffer, sizeof(buffer), "[%s][%s] %s (%d)", DEBUG_TAG, local_tag, str, value);
    system_print(buffer);
}

void log_error(const char* local_tag, const char* str, int value) {
    char buffer[LOG_BUFFER_SIZE];
    snprintf(buffer, sizeof(buffer), "[%s][%s] %s (%d)", ERROR_TAG, local_tag, str, value);
    system_print(buffer);
}
