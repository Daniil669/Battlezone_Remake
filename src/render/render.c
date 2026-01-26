#include <stdio.h>
#include <unistd.h>
#include <math.h>

#include <SDL2/SDL_error.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>

#include "render.h"
#include "game/game.h"

#define SCREEN_W 1600
#define SCREEN_H 900

#define M_PI 3.1415926

static void draw_thick_line(SDL_Renderer *r, float x1, float y1, float x2, float y2, float thick) {
    SDL_SetRenderDrawColor(r, 0, 255, 80, 255);
    for (int dx = -thick/2; dx <= thick/2; dx++) {
        for (int dy = -thick/2; dy <= thick/2; dy++) {
            SDL_RenderDrawLineF(r, x1+dx, y1+dy, x2+dx, y2+dy);
        }
    }
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

static float normalize_angle(float angle) {
    while (angle < 0.0f) angle += 2.0f * (float)M_PI;
    while (angle >= 2.0f * (float)M_PI) angle -= 2.0f * (float)M_PI;
    return angle;
}

static void draw_hud(SDL_Renderer *renderer, Uint32 ticks, float angle) {
    // horizontal line
    draw_thick_line(renderer, 0, 100, 745, 100, 1);
    draw_thick_line(renderer, 855, 100, SCREEN_W, 100, 1);

    // radar + cross + V 
    int reticle_cx = SCREEN_W / 2;
    int reticle_cy = 100;

    SDL_SetRenderDrawColor(renderer, 0, 255, 80, 255);
    int x = 65;
    int y = 0;
    int err = 1 - x;

    while (x >= y) {
        SDL_RenderDrawPoint(renderer, reticle_cx + y, reticle_cy + x);
        SDL_RenderDrawPoint(renderer, reticle_cx + x, reticle_cy + y);
        SDL_RenderDrawPoint(renderer, reticle_cx - y, reticle_cy + x);
        SDL_RenderDrawPoint(renderer, reticle_cx - x, reticle_cy + y);
        SDL_RenderDrawPoint(renderer, reticle_cx - x, reticle_cy - y);
        SDL_RenderDrawPoint(renderer, reticle_cx - y, reticle_cy - x);
        SDL_RenderDrawPoint(renderer, reticle_cx + y, reticle_cy - x);
        SDL_RenderDrawPoint(renderer, reticle_cx + x, reticle_cy - y);

        y++;
        if (err <= 0) {
            err += 2 * y + 1;
        } else {
            x--;
            err += 2 * (y - x) + 1;
        }
    }

    // Cross
    draw_thick_line(renderer, reticle_cx, 35, reticle_cx, 45, 1);
    draw_thick_line(renderer, reticle_cx, reticle_cy + 55, reticle_cx, reticle_cy + 65, 1);

    // V shape inside circle
    // Central ray — exactly where player is looking
    float player_angle_rad = angle * M_PI / 180.0f;

    // Left side ray: -22.5° from center
    float left_angle = player_angle_rad - 22.5f * M_PI / 180.0f;
    float left_x = reticle_cx + 65 * cosf(left_angle);
    float left_y = reticle_cy - 65 * sinf(left_angle);

    draw_thick_line(renderer, reticle_cx, reticle_cy, (int)left_x, (int)left_y, 1);

    // Right side ray: +22.5° from center
    float right_angle = player_angle_rad + 22.5f * M_PI / 180.0f;
    float right_x = reticle_cx + 65 * cosf(right_angle);
    float right_y = reticle_cy - 65 * sinf(right_angle);

    draw_thick_line(renderer, reticle_cx, reticle_cy, (int)right_x, (int)right_y, 1);

    // scan line
    float sweep_progress = (float)ticks * 0.001f;  // 1 full rotation per second
    float sweep_angle = fmodf(sweep_progress, 2.0f * M_PI);
    
    float sweep_end_x = reticle_cx + 65 * cosf(sweep_angle);
    float sweep_end_y = reticle_cy + 65 * sinf(sweep_angle);
    // SDL_SetRenderDrawColor(renderer, 255, 255, 100, 200);  // bright yellow sweep
    draw_thick_line(renderer, reticle_cx, reticle_cy, sweep_end_x, sweep_end_y, 1);


    // float center_x = reticle_cx + 65 * cosf(player_angle_rad);
    // float center_y = reticle_cy - 65 * sinf(player_angle_rad);  // minus because SDL y grows downward

    // draw_thick_line(renderer, reticle_cx, reticle_cy, (int)center_x, (int)center_y, 1);

}



void draw_home(SDL_Renderer *renderer, TTF_Font *font_title, TTF_Font *font_small, color_t *colors, Uint32 ticks, float angle) {
    // Black background
    SDL_SetRenderDrawColor(renderer, colors[BLACK].r, colors[BLACK].g, colors[BLACK].b, 255);
    // SDL_SetRenderDrawColor(renderer, 21, 23, 23, 255);
    SDL_RenderClear(renderer);

    draw_hud(renderer, ticks, angle);

    // "TANKS 2D" title 
    if (font_title) {
        SDL_Color col = {40, 252, 72, 255};
        const char *title = "TANKS 2D";

        SDL_Surface *surf = TTF_RenderText_Blended(font_title, title, col);
        if (surf) {
            SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surf);
            if (tex) {
                int w = surf->w;
                int h = surf->h;
                SDL_Rect dst = {SCREEN_W/2 - w/2, 260, w, h};
                SDL_RenderCopy(renderer, tex, NULL, &dst);
                SDL_DestroyTexture(tex);
            }
            SDL_FreeSurface(surf);
        }
    }

    // Instructions block 
    if (font_small) {
        SDL_Color col = {40, 252, 72, 255};
        const char *lines[] = {
            "'W' - MOVE FORWARD; 'S' - MOVE BACKWARD",
            "'A' - ROTATE LEFT; 'D' - ROTATE RIGHT",
            "'SPACE' - SHOOT",
            "'ESC' - EXIT",
            "",
            "PRESS 'ENTER' TO START"
        };

        int y = 480;
        const int line_spacing = 38;

        for (size_t i = 0; i < sizeof(lines)/sizeof(lines[0]); i++) {
            SDL_Surface *surf = TTF_RenderText_Blended(font_small, lines[i], col);
            if (surf) {
                SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surf);
                if (tex) {
                    SDL_Rect dst = {SCREEN_W/2 - surf->w/2, y, surf->w, surf->h};
                    SDL_RenderCopy(renderer, tex, NULL, &dst);
                    SDL_DestroyTexture(tex);
                }
                SDL_FreeSurface(surf);
            }
            y += line_spacing;
        }
    }

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


void render(game_state_t *gs, render_t* render_obj, Uint32 ticks) {

    switch (gs->game_stage) {
        case HOME:
            draw_home(render_obj->renderer, render_obj->big_font, render_obj->small_font, render_obj->colors, ticks, 90.0);
        case GAME_ON:
            break;
        case GAME_OVER:
            break;
        case INPUT_NAME:
            break;
        case SHOW_SCORES:
            break;
    }

}

static void populate_colors(color_t *colors, int *rgb_values) {
// #define GREEN {}
// #define RED {}
// #define BLACK {}
// #define ORANGE {}
// #define YELLOW {}
// #define WHITE {}
// #define PINK {}
// #define BLUE {}
    for (int i = 0; i < 8; i++) {
        int rgb_v_index = 3 * i;
        colors[i].r = rgb_values[rgb_v_index];
        colors[i].g = rgb_values[rgb_v_index+1];
        colors[i].b = rgb_values[rgb_v_index+2];
    }
}

render_t *render_init() {
    render_t *render_obj = malloc(sizeof(render_t));
    if (render_obj == NULL) {
        return NULL;
    }

    const char *title = "Tanks 2D";
    SDL_Window *window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_W, SCREEN_H, 0);
    if (window == NULL) {
        printf("Error: %s\n", SDL_GetError());
        SDL_ClearError();
        return NULL;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Error: %s\n", SDL_GetError());
        SDL_ClearError();
        return NULL;
    }

    TTF_Font *font_small = TTF_OpenFont("./assets/batmfa__.ttf", 16);
    TTF_Font *font_big = TTF_OpenFont("./assets/batmfa__.ttf", 96);
    if (!font_small || !font_big) {
        printf("Error: %s\n", TTF_GetError());
        SDL_ClearError();
        return NULL;
    }

    color_t *colors = malloc(8 * sizeof(color_t));
    if (colors == NULL) {
        printf("Error: colors allocation faild.\n");
        return NULL;
    }
    int rgb_values[24] = {0, 255, 80, 255, 0, 0, 21, 23, 23, 255, 123, 0, 242, 255, 0, 255, 255, 255, 255, 0, 238, 0, 255, 234};
    populate_colors(colors, rgb_values);

    render_obj->window = window;
    render_obj->renderer = renderer;
    render_obj->big_font = font_big;
    render_obj->small_font = font_small;
    render_obj->colors = colors;
    return render_obj;
}

void render_free(render_t *render_obj) {
    TTF_CloseFont(render_obj->big_font);
    TTF_CloseFont(render_obj->small_font);
    SDL_DestroyRenderer(render_obj->renderer);
    SDL_DestroyWindow(render_obj->window);
    free(render_obj->colors);
    free(render_obj);

}