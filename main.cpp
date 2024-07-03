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

#include <stdio.h>
#include <SDL2/SDL.h>

#define WINDOW_WIDTH            640
#define WINDOW_HEIGHT           480

int main(int argc, char* args[]) {
    SDL_Window* mainFrame = NULL;
    //SDL_Surface* screenSurface = NULL;

    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("INFO [SDL_INIT] Could not init video, code: %d\n", SDL_GetError());
        return -1;
    }
    printf("INFO [SDL_INIT] SDL Inited\n");

    mainFrame = SDL_CreateWindow("Animals War", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

    if(mainFrame == NULL) {
        printf("INFO [WNDW] Could not create window\n");
        return -1;
    }
    printf("INFO [WNDW] Created window\n");
    SDL_Renderer* renderer = SDL_CreateRenderer(mainFrame, -1, 0);

    //screenSurface = SDL_GetWindowSurface(mainFrame);
    // SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xff, 0xff, 0xff));
    // SDL_UpdateWindowSurface(mainFrame);

    bool quit = false;
    float speed = 0;
    float w = WINDOW_WIDTH / 2 - 50;

    while(!quit) {
        SDL_Event e;
        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_QUIT) {
                quit = true;
            }
            if(e.type == SDL_KEYUP) {
                speed = 0;
            }else if(e.type == SDL_KEYDOWN) {
                if(e.key.keysym.scancode == SDL_SCANCODE_LEFT) {
                    speed = -0.1;
                }else if(e.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
                    speed = 0.1;
                }
            }
        }

        w += speed;

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawLine(renderer, 0, 380, 640, 380);

        SDL_Rect player = {w, 330, 50, 50};
        SDL_RenderDrawRect(renderer, &player);
        SDL_RenderPresent( renderer );
    }

    SDL_DestroyWindow(mainFrame);
    printf("INFO [WNDW] Destroyed window\n");
    SDL_Quit();

    return 0;
}