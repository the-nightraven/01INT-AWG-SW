/*
###########################################################

@file: main.cpp

@author: Tanasescu Darius

@Licence: Apache2.0

@Copyleft the-nightraven, product is open source and has
absolutely no warranty, you are free to copy, distribute
and change, but not for commercial use

@Revision: InfDev

@Description: Main entrypoint in application

###########################################################
*/

#include "app/app.h"
#define MAIN_TAG       "MAIN"

//external definitions
void system_print(char* str) {
    printf("%s\n", str);
}

int main(int argc, char* args[]) {
    G_STATUS status;   
    status = app_init();

    if(status == G_STATUS_FAIL) {
        log_error(MAIN_TAG, "Cannot init app", G_STATUS_FAIL);
        return G_STATUS_FAIL;
    }

    status = app_loop();
    if(status == G_STATUS_FAIL) {
        log_error(MAIN_TAG, "Loop returned errors", -1);
    }
    log_info(MAIN_TAG, "Successful loop exit");

    //TODO: verbose deinit
    status = app_deinit();
    if(status == G_STATUS_FAIL) {
        log_error(MAIN_TAG, "Error on deinit", -1);
    }
    log_info(MAIN_TAG, "Successful deinit");

    return status;
}