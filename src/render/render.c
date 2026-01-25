#include <stdio.h>
#include <unistd.h>
#include <math.h>

#include <SDL2/SDL_error.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>

#include "render.h"
#include "game/game.h"

#define SCREEN_W 1280
#define SCREEN_H 720
#define GREEN_R 40
#define GREEN_G 252
#define GREEN_B 72
#define GREEN_A 255

static void draw_line(SDL_Renderer *renderer, float x1, float y1, float x2, float y2) {
    SDL_RenderDrawLine(renderer, (int)x1, (int)y1, (int)x2, (int)y2);
}

static void draw_normal_tank(SDL_Renderer *r, float x, float y, float s) {

}


static void draw_super_tank(SDL_Renderer *r, float x, float y, float s) {
    
}


static void draw_missile(SDL_Renderer *r, float x, float y, float s) {
    
}


static void draw_ufo(SDL_Renderer *r, float cx, float cy, float s) {
    
}



static void draw_bullet(SDL_Renderer *r, float x, float y, float s)
{
   
}



// Main title render function
void draw_home(SDL_Renderer* renderer, TTF_Font* font_small, Uint32 ticks) {
    SDL_SetRenderDrawColor(renderer, 21, 23, 23, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, GREEN_R, GREEN_G, GREEN_B, GREEN_A);

    // Instructions text
    if (font_small) {
        SDL_Color col = {GREEN_R, GREEN_G, GREEN_B, GREEN_A};
        const char* lines[] = {
            "\"w\" - move forward; \"s\" - move backward;",
            "\"a\" - rotate left; \"d\" - rotate right;",
            "\"Space\" - shoot;",
            "\"ESC\" - exit;",
            "",
            "Press \"Enter\" to start"
        };

        int y = (SCREEN_H / 2 + 80) + 100;
        for (int i = 0; i < 6; i++) {
            SDL_Surface* surf = TTF_RenderText_Blended(font_small, lines[i], col);
            if (!surf) continue;
            SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
            if (tex) {
                int w = surf->w, h = surf->h;
                SDL_Rect dst = {SCREEN_W/2 - w/2, y, w, h};
                SDL_RenderCopy(renderer, tex, NULL, &dst);
                SDL_DestroyTexture(tex);
            }
            SDL_FreeSurface(surf);
            y += 32;
        }
    }

    // Optional pulsing "Press Enter" emphasis
    float pulse = 0.7f + 0.3f * sinf(ticks * 0.004f);
    SDL_SetRenderDrawColor(renderer, GREEN_R, (Uint8)(GREEN_G * pulse), (Uint8)(GREEN_B * pulse), GREEN_A);
    // You can re-draw "Press Enter" line here with pulse if you want stronger effect

    SDL_RenderPresent(renderer);
}


static void draw_game_on() {

}

static void draw_game_over() {

}

static void draw_input_name() {

}

static void draw_scores_list() {

}


void render(game_state_t *gs) {
    const char *title = "Battlezone";
    SDL_Window *window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_W, SCREEN_H, SDL_WINDOW_FULLSCREEN_DESKTOP);
    if (window == NULL) {
        printf("Error: %s\n", SDL_GetError());
        SDL_ClearError();
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Error: %s\n", SDL_GetError());
        SDL_ClearError();
    }

    TTF_Font *font = TTF_OpenFont("./assets/batmfa__.ttf", 16);
    if (!font) {
        printf("Error: %s\n", TTF_GetError());
    }
    

    SDL_SetRenderDrawColor(renderer, 210, 23, 23, 255);// black background
    SDL_RenderClear(renderer);

    Uint32 ticks = SDL_GetTicks();
    
    switch (gs->game_stage) {
        case HOME:
            draw_home(renderer, font, ticks);
        case GAME_ON:
            break;
        case GAME_OVER:
            break;
        case INPUT_NAME:
            break;
        case SHOW_SCORES:
            break;
    }


    SDL_RenderPresent(renderer);
    sleep(10);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}