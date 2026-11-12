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

#include "app.h"
#include <cstdio>

// Only include Windows headers if compiling natively on Windows
#if defined(_WIN32) && !defined(DEBUG)
#include <windows.h>
#define SDL_MAIN_HANDLED
#endif

/*
- Debug tag for MAIN
 */
#define MAIN_TAG "MAIN"

/*
- @Desc: printf wrapper for current system
- @Param: str (char*) -> string to be printed
- @returns: (void)
 */
void system_print(const char* str) {
    printf("%s\n", str);
}

/*
- Core application routine shared between entry points
 */
static int run_app() {
    G_STATUS status = app_init();

    if (status == G_STATUS_FAIL) {
        log_error(MAIN_TAG, "Cannot init app", G_STATUS_FAIL);
        return G_STATUS_FAIL;
    }

    status = app_loop();
    if (status == G_STATUS_FAIL) {
        log_error(MAIN_TAG, "Loop returned errors", -1);
    }
    log_info(MAIN_TAG, "Successful loop exit");

    status = app_deinit();
    if (status == G_STATUS_FAIL) {
        log_error(MAIN_TAG, "Error on deinit", -1);
    }
    log_info(MAIN_TAG, "Successful deinit");

    return status;
}

/*
- Windows Release GUI Entrypoint (Hides Console Window on Windows)
 */
#if defined(_WIN32) && !defined(DEBUG)

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    (void)hInstance;
    (void)hPrevInstance;
    (void)lpCmdLine;
    (void)nCmdShow;

    SDL_SetMainReady();
    return run_app();
}

/*
- Standard Entrypoint for Linux (Debug & Release) and Windows (Debug)
 */
#else

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    return run_app();
}

#endif