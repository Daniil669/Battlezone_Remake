#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>

#include "render/render.h"
#include "input/input.h"
#include "game/game.h"

int main() {
    int init = SDL_InitSubSystem(SDL_INIT_VIDEO);
    if (init != 0) {
        printf("Error: %s\n", SDL_GetError());
        SDL_ClearError();
        return 1;
    }

    init = SDL_InitSubSystem(SDL_INIT_TIMER);
    if (init != 0) {
        printf("Error: %s\n", SDL_GetError());
        SDL_ClearError();
        return 1;
    }

    render();
    // while (1) {
    //     input();
    //     update();
    //     render();
    // }

    SDL_QuitSubSystem(SDL_INIT_VIDEO);
    SDL_QuitSubSystem(SDL_INIT_TIMER);
    SDL_Quit();

    return 0;
}