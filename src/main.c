#include <stdio.h>
#include "entities/entities.h"

int main() {
    entety_t *player;
    if (spawn(&player, PLAYER)) {
        printf("Failed to create player");
        return 1;
    }
    printf("Player:\nx: %f, y:%f, angle: %f\n", player->position_x, player->position_y, player->angle);
    printf("moving speed: %f, rotating speed: %f\n", player->moving_speed, player->rotating_speed);
    printf("lives: %i, score: %i\n", player->lives, player->score);
    free(player);
    return 0;
}