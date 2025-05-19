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

/*
 * Main import file
 */
#include "app.h"


/*
 * Production profile includes
 */
#ifndef DEBUG
#include <windows.h>
#define SDL_MAIN_HANDLED
#endif

/*
 * Debug tag for MAIN
 */
#define MAIN_TAG       "MAIN"


/*
 * @Desc: printf wrapper for current system
 * @Param: str (char*) -> string to be printed
 * @returns: (void)
 */
void system_print(char* str) {
    printf("%s\n", str);
}

/*
 * Main entrypoint for DEBUG profile
 */
#if DEBUG

/*
 * @Desc: main entrypoint
 * @Param: argc (int) -> CLI argument length
 *         argv (char**) -> CLI arguments
 * @Returns: status (int) -> exit status
 */
int main(int argc, char* args[]) {
    G_STATUS status = app_init();

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


/*
 * Main entrypoint for Prod profile
 */
#else

/*
 * @Desc: Main entrypoint for Production Profile for Windows
 * @Param: hInstance (HINSTANCE) -> current window instance handler
 *         hPrevInstance (HINSTANCE) -> previous window instance handler
 *         lpCmdLine (LPSTR) -> CLI parameters
 *         nCmdShow (int) -> show/hide CLI Terminal
 * @Returns: status (int WINAPI) -> exit status
 */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    SDL_SetMainReady();

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

#endif