#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>
#include "render.h"
#include "game/game.h"

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

static void draw_mountains(SDL_Renderer *r, int w, int h)
{
    SDL_SetRenderDrawColor(r, 0, 200, 0, 255);

    int y = h / 3;
    int x = 0;

    while (x < w) {
        int nx = x + 80;
        int ny = y + (rand() % 80 - 40);
        draw_line(r, x, y, nx, ny);
        x = nx;
        y = ny;
    }

    // Horizon
    draw_line(r, 0, h / 3, w, h / 3);
}

static void draw_battlezone_logo(SDL_Renderer *r, int cx, int cy, int s)
{
    SDL_SetRenderDrawColor(r, 0, 255, 0, 255);

    int x = cx - 240 * s;
    int y = cy;

    // B
    draw_line(r, x, y, x, y + 60*s);
    draw_line(r, x, y, x + 30*s, y + 10*s);
    draw_line(r, x + 30*s, y + 10*s, x, y + 30*s);
    draw_line(r, x, y + 30*s, x + 30*s, y + 50*s);
    draw_line(r, x + 30*s, y + 50*s, x, y + 60*s);
    x += 50*s;

    // A
    draw_line(r, x, y + 60*s, x + 20*s, y);
    draw_line(r, x + 20*s, y, x + 40*s, y + 60*s);
    draw_line(r, x + 10*s, y + 35*s, x + 30*s, y + 35*s);
    x += 60*s;

    // T
    draw_line(r, x, y, x + 40*s, y);
    draw_line(r, x + 20*s, y, x + 20*s, y + 60*s);
    x += 60*s;

    // T
    draw_line(r, x, y, x + 40*s, y);
    draw_line(r, x + 20*s, y, x + 20*s, y + 60*s);
    x += 60*s;

    // L
    draw_line(r, x, y, x, y + 60*s);
    draw_line(r, x, y + 60*s, x + 40*s, y + 60*s);
    x += 60*s;

    // E
    draw_line(r, x, y, x, y + 60*s);
    draw_line(r, x, y, x + 40*s, y);
    draw_line(r, x, y + 30*s, x + 30*s, y + 30*s);
    draw_line(r, x, y + 60*s, x + 40*s, y + 60*s);
    x += 60*s;

    // Z O N E (simplified)
    draw_line(r, x, y, x + 40*s, y + 60*s);        // Z
    draw_line(r, x + 40*s, y + 60*s, x, y + 60*s);
    x += 60*s;

    draw_line(r, x, y, x + 40*s, y);
    draw_line(r, x + 40*s, y, x + 40*s, y + 60*s);
    draw_line(r, x + 40*s, y + 60*s, x, y + 60*s);
    draw_line(r, x, y + 60*s, x, y);
}

static void draw_text(SDL_Renderer *r, int x, int y)
{
    SDL_SetRenderDrawColor(r, 0, 200, 0, 255);

    draw_line(r, x, y, x + 140, y); // underline

    draw_line(r, x, y + 20, x + 160, y + 20);
    draw_line(r, x, y + 40, x + 200, y + 40);
    draw_line(r, x, y + 60, x + 120, y + 60);
}


static void draw_home_screen(SDL_Renderer *renderer, int width, int height)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    draw_mountains(renderer, width, height);
    draw_battlezone_logo(renderer, width / 2, height / 2 - 40, 1);
    draw_text(renderer, width / 2 - 100, height / 2 + 80);

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
    

    SDL_SetRenderDrawColor(render_obj, 0, 0, 0, 255);// black background
    SDL_RenderClear(render_obj);
    
    switch (gs->game_stage) {
        case HOME:
            draw_home_screen(render_obj, 1280, 720);
        case GAME_ON:
            break;
        case GAME_OVER:
            break;
        case INPUT_NAME:
            break;
        case SHOW_SCORES:
            break;
    }


    SDL_RenderPresent(render_obj);
    sleep(10);

    SDL_DestroyRenderer(render_obj);
    SDL_DestroyWindow(window);
}