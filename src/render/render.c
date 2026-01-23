#include <stdio.h>
#include <unistd.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>
#include "render.h"


void render() {
    const char *title = "Battlezone";
    SDL_Window *window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, 0);
    if (window == NULL) {
        printf("Error: %s", SDL_GetError());
        SDL_ClearError();
    }
    SDL_Renderer *render_obj = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (render_obj == NULL) {
        printf("Error: %s", SDL_GetError());
        SDL_ClearError();
    }
    
    SDL_SetRenderDrawColor(render_obj, 0, 0, 0, 255);     // black background
    SDL_RenderClear(render_obj);

    SDL_SetRenderDrawColor(render_obj, 42, 199, 99, 1);
    SDL_RenderDrawLine(render_obj, 0, 0, 50, 50);


    SDL_RenderPresent(render_obj);
    sleep(5);


    SDL_DestroyRenderer(render_obj);
    SDL_DestroyWindow(window);
}