#include <stdio.h>
#include <unistd.h>
#include <string.h>
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

static void draw_thick_line(SDL_Renderer *r, float x1, float y1, float x2, float y2, float thick, color_t color) {
    SDL_SetRenderDrawColor(r, color.r, color.g, color.b, 255);
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


static void draw_hud(GameStage gs, SDL_Renderer *renderer, Uint32 ticks, float angle, color_t *colors,  TTF_Font *medium_font) {
    int score_int = 69069;
    int lives_int = 3;

    if (gs != SHOW_SCORES && medium_font) { // gs != HOME
        SDL_Color col = {colors[BLUE].r, colors[BLUE].g, colors[BLUE].b, 255};

        int x = 1050;
        int y = 20;
        int line_spacing = 8; // space between lines

        // Enemy in range
        // if (enemy is inside player's fov)
        const char *enemy_warning = "Enemy in range";

        SDL_Surface *surf = TTF_RenderText_Blended(medium_font, enemy_warning, col);
        if (surf) {
            SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surf);
            if (tex) {
                SDL_Rect dst = {100, 35, surf->w, surf->h}; // rectangle where the text is drawn
                SDL_RenderCopy(renderer, tex, NULL, &dst);
                SDL_DestroyTexture(tex);
            }
            SDL_FreeSurface(surf);
        }

        /* -------- Lives -------- */
        char lives_text[32];
        snprintf(lives_text, sizeof(lives_text), "Lives: %d", lives_int);

        surf = TTF_RenderText_Blended(medium_font, lives_text, col);
        if (surf) {
            SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surf);
            if (tex) {
                SDL_Rect dst = {x, y, surf->w, surf->h};
                SDL_RenderCopy(renderer, tex, NULL, &dst);

                y += surf->h + line_spacing;  // move down for next line
                SDL_DestroyTexture(tex);
            }
            SDL_FreeSurface(surf);
        }

        /* -------- Score -------- */
        char score_text[32];
        snprintf(score_text, sizeof(score_text), "Score: %d", score_int);

        surf = TTF_RenderText_Blended(medium_font, score_text, col);
        if (surf) {
            SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surf);
            if (tex) {
                SDL_Rect dst = {x, y, surf->w, surf->h};
                SDL_RenderCopy(renderer, tex, NULL, &dst);
                SDL_DestroyTexture(tex);
            }
            SDL_FreeSurface(surf);
        }
    }


    // horizontal line
    draw_thick_line(renderer, 0, 100, 745, 100, 1, colors[GREEN]);
    draw_thick_line(renderer, 855, 100, SCREEN_W, 100, 1, colors[GREEN]);

    // radar + cross + V 
    int reticle_cx = SCREEN_W / 2;
    int reticle_cy = 100;

    SDL_SetRenderDrawColor(renderer, colors[GREEN].r, colors[GREEN].g, colors[GREEN].b, 255);
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
    draw_thick_line(renderer, reticle_cx, 35, reticle_cx, 45, 1, colors[GREEN]);
    draw_thick_line(renderer, reticle_cx, reticle_cy + 55, reticle_cx, reticle_cy + 65, 1, colors[GREEN]);

    // V shape inside circle
    // Central ray — exactly where player is looking
    float player_angle_rad = angle * M_PI / 180.0f;

    // Left side ray: -22.5° from center
    float left_angle = player_angle_rad - 22.5f * M_PI / 180.0f;
    float left_x = reticle_cx + 65 * cosf(left_angle);
    float left_y = reticle_cy - 65 * sinf(left_angle);

    draw_thick_line(renderer, reticle_cx, reticle_cy, (int)left_x, (int)left_y, 1, colors[GREEN]);

    // Right side ray: +22.5° from center
    float right_angle = player_angle_rad + 22.5f * M_PI / 180.0f;
    float right_x = reticle_cx + 65 * cosf(right_angle);
    float right_y = reticle_cy - 65 * sinf(right_angle);

    draw_thick_line(renderer, reticle_cx, reticle_cy, (int)right_x, (int)right_y, 1, colors[GREEN]);

    // scan line
    float sweep_progress = (float)ticks * 0.001f;  // 1 full rotation per second
    float sweep_angle = fmodf(sweep_progress, 2.0f * M_PI);
    
    float sweep_end_x = reticle_cx + 65 * cosf(sweep_angle);
    float sweep_end_y = reticle_cy + 65 * sinf(sweep_angle);
    // SDL_SetRenderDrawColor(renderer, 255, 255, 100, 200);  // bright yellow sweep
    draw_thick_line(renderer, reticle_cx, reticle_cy, sweep_end_x, sweep_end_y, 1, colors[GREEN]);

}



void draw_home(GameStage gs, SDL_Renderer *renderer, TTF_Font *font_title, TTF_Font *font_small, TTF_Font *medium_font, color_t *colors, Uint32 ticks, float angle) {
    // Black background
    SDL_SetRenderDrawColor(renderer, colors[BLACK].r, colors[BLACK].g, colors[BLACK].b, 255);
    // SDL_SetRenderDrawColor(renderer, 21, 23, 23, 255);
    SDL_RenderClear(renderer);

    draw_hud(gs, renderer, ticks, angle, colors, medium_font);

    // "TANKS 2D" title 
    if (font_title) {
        SDL_Color col = {colors[GREEN].r, colors[GREEN].g, colors[GREEN].b, 255};
        const char *title = "TANKS 2D";

        SDL_Surface *surf = TTF_RenderText_Blended(font_title, title, col);
        if (surf) {
            SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surf);
            if (tex) {
                int w = surf->w;
                int h = surf->h;
                SDL_Rect dst = {SCREEN_W/2 - w/2, 260, w, h}; // rectangle where the text is drawn
                SDL_RenderCopy(renderer, tex, NULL, &dst);
                SDL_DestroyTexture(tex);
            }
            SDL_FreeSurface(surf);
        }
    }

    // Instructions block 
    if (font_small) {
        SDL_Color col = {colors[GREEN].r, colors[GREEN].g, colors[GREEN].b, 255};
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

static void draw_text(SDL_Renderer *renderer, TTF_Font *font, const char *text, int x, int y, SDL_Color color) {

    SDL_Surface *surf = TTF_RenderText_Blended(font, text, color);
    if (!surf) return;

    SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surf);
    if (!tex) {
        SDL_FreeSurface(surf);
        return;
    }

    SDL_Rect dst = { x, y, surf->w, surf->h };
    SDL_RenderCopy(renderer, tex, NULL, &dst);

    SDL_DestroyTexture(tex);
    SDL_FreeSurface(surf);
}

static void draw_scores_list(SDL_Renderer *renderer, color_t *colors, TTF_Font *medium_font, TTF_Font *small_font, score_entry_t *scores) {

    SDL_SetRenderDrawColor(renderer, colors[BLACK].r, colors[BLACK].g, colors[BLACK].b, 255);
    SDL_RenderClear(renderer);

    SDL_Color green = {
        colors[GREEN].r,
        colors[GREEN].g,
        colors[GREEN].b,
        255
    };

    SDL_Color red = {
        colors[RED].r,
        colors[RED].g,
        colors[RED].b,
        255
    };

    /* ---------- Frame ---------- */
    const int frame_w = 600;
    const int frame_h = 520;

    SDL_Rect frame = {
        SCREEN_W / 2 - frame_w / 2,
        100,
        frame_w,
        frame_h
    };

    SDL_SetRenderDrawColor(renderer, green.r, green.g, green.b, 255);
    SDL_RenderDrawRect(renderer, &frame);

    /* ---------- Title ---------- */
    draw_text(renderer, medium_font,
              "TOP 10",
              frame.x + frame.w / 2 - 60,
              frame.y + 25,
              green);

    /* ---------- Column headers ---------- */
    int start_y = frame.y + 90;

    draw_text(renderer, medium_font, "PLACE", frame.x + 60,  start_y, green);
    draw_text(renderer, medium_font, "NAME",  frame.x + 260, start_y, green);
    draw_text(renderer, medium_font, "SCORE", frame.x + 450, start_y, green);

    /* ---------- Divider lines ---------- */
    SDL_RenderDrawLine(renderer,
                       frame.x + 200, start_y - 10,
                       frame.x + 200, frame.y + frame.h - 70);

    SDL_RenderDrawLine(renderer,
                       frame.x + 400, start_y - 10,
                       frame.x + 400, frame.y + frame.h - 70);

    /* ---------- Rows ---------- */
    int row_y = start_y + 40;
    int row_step = 28;

    for (int i = 0; i < TOP_SCORES; i++) {
        char place[4];
        char score_text[12];

        snprintf(place, sizeof(place), "%d", i + 1);
        snprintf(score_text, sizeof(score_text), "%d", scores[i].score);

        draw_text(renderer, small_font,
                  place,
                  frame.x + 100,
                  row_y,
                  green);

        draw_text(renderer, small_font,
                  scores[i].name,
                  frame.x + 250,
                  row_y,
                  green);

        draw_text(renderer, small_font,
                  score_text,
                  frame.x + 450,
                  row_y,
                  green);

        row_y += row_step;
    }

    /* ---------- Footer ---------- */
    draw_text(renderer, medium_font,
              "PRESS \"ENTER\"",
              SCREEN_W / 2 - 105,
              frame.y + frame.h + 30,
              red);

    SDL_RenderPresent(renderer);
}


static void draw_name_entry(SDL_Renderer *renderer, color_t *colors, TTF_Font *font_medium, TTF_Font *font_small, int top_y) {
    SDL_Color green  = { colors[GREEN].r,  colors[GREEN].g,  colors[GREEN].b, 255 };
    SDL_Color yellow = { colors[YELLOW].r, colors[YELLOW].g, colors[YELLOW].b, 255 };

    const int frame_w = 800;
    const int frame_h = 460;

    SDL_Rect frame = {SCREEN_W / 2 - frame_w / 2, top_y, frame_w, frame_h};

    SDL_SetRenderDrawColor(renderer, green.r, green.g, green.b, 255);
    SDL_RenderDrawRect(renderer, &frame);

    /* -------- Title -------- */
    draw_text(renderer, font_medium, "ENTER NAME", frame.x + 300, frame.y + 30, green);

    /* -------- Player name -------- */
    draw_text(renderer, font_medium, "JOHNNY", frame.x + 60, frame.y + 150, green);

    /* -------- Input underline -------- */
    SDL_RenderDrawLine(renderer, frame.x + 60, frame.y + 185, frame.x + frame.w - 60, frame.y + 185);

    /* -------- Warning text -------- */
    draw_text(renderer, font_small, "NAME ALREADY TAKEN", frame.x + 280, frame.y + 280, yellow);

    draw_text(renderer, font_small, "CHOOSE ANOTHER ONE", frame.x + 280, frame.y + 320, yellow);
}



static void draw_game_over(GameStage gs, SDL_Renderer *renderer, color_t *colors, TTF_Font *font_title, TTF_Font *font_small, TTF_Font *font_medium)
{
    SDL_SetRenderDrawColor(renderer, colors[BLACK].r, colors[BLACK].g, colors[BLACK].b, 255);
    SDL_RenderClear(renderer);

    int game_over_bottom = 0;

    if (font_title) {
        SDL_Color col = { colors[GREEN].r, colors[GREEN].g, colors[GREEN].b, 255
        };

        SDL_Surface *surf = TTF_RenderText_Blended(font_title, "GAME OVER", col);
        if (surf) {
            SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surf);
            if (tex) {
                SDL_Rect dst = { SCREEN_W / 2 - surf->w / 2, 100, surf->w, surf->h }; //SCREEN_H / 2 - surf->h - 60

                SDL_RenderCopy(renderer, tex, NULL, &dst);
                game_over_bottom = dst.y + dst.h;

                SDL_DestroyTexture(tex);
            }
            SDL_FreeSurface(surf);
        }
    }

    // sleep(3);

    // SDL_SetRenderDrawColor(renderer, colors[BLACK].r, colors[BLACK].g, colors[BLACK].b, 255);
    // SDL_RenderClear(renderer);

    /* Box appears nicely below GAME OVER */
    draw_name_entry(renderer, colors, font_medium, font_small, game_over_bottom + 30);

    SDL_RenderPresent(renderer);
}




void render(game_state_t *gs, render_t* render_obj, Uint32 ticks) {
    score_entry_t scores[TOP_SCORES] = {
        {"KING", 696969},
        {"KING", 696969},
        {"KING", 696969},
        {"KING", 696969},
        {"KING", 696969},
        {"PING", 690000},
        {"KINGS",690000},
        {"KINGV",690000},
        {"KINGD",600000},
        {"KINGB",600000}
    };

    switch (gs->game_stage) {
        case HOME:
            draw_home(gs->game_stage, render_obj->renderer, render_obj->big_font, render_obj->small_font, render_obj->medium_font, render_obj->colors, ticks, 90.0);
        case GAME_ON:
            break;
        case GAME_OVER:
            draw_game_over(gs->game_stage, render_obj->renderer, render_obj->colors, render_obj->big_font, render_obj->small_font, render_obj->medium_font);
            break;
        case SHOW_SCORES:
            draw_scores_list(render_obj->renderer, render_obj->colors, render_obj->medium_font, render_obj->small_font, scores);
            break;
    }

}

static void populate_colors(color_t *colors, int *rgb_values) {
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
    TTF_Font *medium_font = TTF_OpenFont("./assets/batmfa__.ttf", 24);
    if (!font_small || !font_big || !medium_font) {
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
    render_obj->medium_font = medium_font;
    render_obj->small_font = font_small;
    render_obj->colors = colors;
    return render_obj;
}

void render_free(render_t *render_obj) {
    TTF_CloseFont(render_obj->big_font);
    TTF_CloseFont(render_obj->medium_font);
    TTF_CloseFont(render_obj->small_font);
    SDL_DestroyRenderer(render_obj->renderer);
    SDL_DestroyWindow(render_obj->window);
    free(render_obj->colors);
    free(render_obj);

}