#pragma once

#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include "game/game.h"

typedef enum {
    GREEN,
    RED,
    BLACK,
    ORANGE,
    YELLOW,
    WHITE,
    PINK,
    BLUE
} ColorName;

typedef struct {
    int r;
    int g;
    int b;
} color_t;

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    TTF_Font *small_font;
    TTF_Font *big_font;
    color_t *colors;
} render_t;


render_t *render_init();
void render_free(render_t *render_obj);
void render(game_state_t *gs, render_t* render_obj, Uint32 ticks);