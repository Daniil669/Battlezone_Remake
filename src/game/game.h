#pragma once

typedef enum {
    HOME,
    GAME_ON,
    GAME_OVER,
    SHOW_SCORES
} GameStage;

typedef struct {
    GameStage game_stage;
} game_state_t;

void update();
game_state_t *init_game();