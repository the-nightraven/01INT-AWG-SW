/*
###########################################################

@file: debugger.h

@author: Tanasescu Darius

@Licence: Apache2.0

@Copyleft the-nightraven, product is open source and has
absolutely no warranty, you are free to copy, distribute
and change, but not for commercial use

@Revision: InfDev

@Description: Debugger component source file

###########################################################
*/

#include "debugger.h"
#include "pthread.h"

pthread_t debugger_th = 0;
bool debuggerIsRunning = false;

G_STATUS debugger_init() {
    if(debuggerIsRunning || debugger_th != 0) {
        return G_STATUS_FAIL;
    }

    debuggerIsRunning = false;
    debugger_th = 0;
    return G_STATUS_OK;
}

G_STATUS debugger_deinit() {
    if(debuggerIsRunning) {
        return G_STATUS_FAIL;
    }

    debugger_th = 0;
    debuggerIsRunning = false;
    return G_STATUS_OK;
}

//thread lifecycle
G_STATUS debugger_start() {
    if(debuggerIsRunning) {
        return G_STATUS_FAIL;
    }
    debuggerIsRunning = true;
    pthread_create(&debugger_th, NULL, debugger_lifecycle, NULL);
    return G_STATUS_OK;
}

G_STATUS debugger_end() {
    if(!debuggerIsRunning) {
        return G_STATUS_FAIL;
    }
    debuggerIsRunning = false;
    pthread_join(debugger_th, NULL);
    return G_STATUS_OK;
}

void* debugger_lifecycle(void* arg) {
    if(!debuggerIsRunning) {
        return NULL;
    }

    while(debuggerIsRunning) {
        //do stuff
    }

    return NULL;
}

//helpers
bool debugger_is_running() {
    return debuggerIsRunning;
}