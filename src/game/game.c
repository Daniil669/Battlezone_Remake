#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "db/scores.h"
#include "entities/entities.h"
#include "game.h"


game_state_t *init_game() {
    game_state_t *gs = malloc(sizeof(game_state_t));
    if (gs == NULL) {
        return NULL;
    }
    gs->game_stage = HOME;
    return gs;
}








// char *name = "Johnny_S";
// int score = 1000;
// scores_t *scores;
// DbExecCode db_status = handle_scores(name, score, &scores);
// if (db_status == DB_NAME_EXISTS) {
//     printf("Name already taken.\n");
//     return 1;
// } else if (db_status != DB_OK) {
//     printf("DB operation faild\n.");
//     return 1;
// }
// printf("TOP 10:\n");
// printf("PLACE | NAME | SCORE | TIME\n");
// for (int i = 0; i < scores->count; i++) {
//     score_t score = *(scores->data[i]);
//     time_t timestamp = score.timestamp;
//     printf("%i | %s | %i | %s", i+1, score.name, score.score, ctime(&timestamp));
// }
// free_scores(scores);


// entety_t *player;
// float dt = 0.008;
// if (spawn(&player, PLAYER, NULL)) {
//     printf("Failed to create player");
//     return 1;
// }
// printf("PLAYER:\n");
// printf("Object:\nx: %.2f, y: %.2f, angle: %.2f\n", player->position_x, player->position_y, player->angle);
// printf("moving speed: %.2f, rotating speed: %.2f\n", player->moving_speed, player->rotating_speed);
// printf("lives: %i, score: %i\n", player->lives, player->score);
// player->angle = 11.0;
// for (int i = 0; i < 10; i++) {
//     move(player, dt, -1.0);
// }
// printf("Object:\nx: %.2f, y: %.2f, angle: %.2f\n", player->position_x, player->position_y, player->angle);
// for (int i = 0; i < 10; i++) {
//     rotate(player, dt, -1.0);
// }
// printf("Object:\nx: %.2f, y: %.2f, angle: %.2f\n", player->position_x, player->position_y, player->angle);
// free(player);