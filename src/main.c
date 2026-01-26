#include <stdio.h>
#include <unistd.h>

#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_timer.h>

#include "render/render.h"
#include "input/input.h"
#include "game/game.h"

int main() {
    int sdl_init = SDL_InitSubSystem(SDL_INIT_VIDEO);
    if (sdl_init != 0) {
        printf("Error: %s\n", SDL_GetError());
        SDL_ClearError();
        return 1;
    }

    sdl_init = SDL_InitSubSystem(SDL_INIT_TIMER);
    if (sdl_init != 0) {
        printf("Error: %s\n", SDL_GetError());
        SDL_ClearError();
        return 1;
    }

    sdl_init = SDL_InitSubSystem(SDL_INIT_AUDIO);
    if (sdl_init != 0) {
        printf("Error: %s\n", SDL_GetError());
        SDL_ClearError();
        return 1;
    }

    int ttf_init = TTF_Init();
    if (ttf_init != 0) {
        printf("Failed to initialize ttf library.\n");
        return 1;
    }

    game_state_t *game_state = init_game();
    if (game_state == NULL) {
        printf("Failed to initialize the game.\n");
        return 1;
    }

    render_t *render_obj = render_init();
    if (render_obj == NULL) {
        printf("Failed to initialize render\n");
    }
    
    for (int i = 0; i < 30; i++) {
        Uint32 ticks = SDL_GetTicks();
        render(game_state, render_obj, ticks);
        sleep(1);
    }

    // while (1) {
    //     input();
    //     update();
    //     render(game_state);
    // }
    free(game_state);
    render_free(render_obj);
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
    SDL_QuitSubSystem(SDL_INIT_TIMER);
    SDL_Quit();

    return 0;
}