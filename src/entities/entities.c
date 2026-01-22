#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include "entities.h"

static void angle_position_calc(float *angle, float *x, float *y, entety_t *entity_obj) {
    const float PI = 3.1415;
    // const int FOV = 90; // 90 deg field of view
    const float SPAWN_DIST = 1200.0;

    float spawn_angle;
    float diff;

    srand(time(NULL));

    int found = 0;
    int count = 0;
    while (found != 1 && count < 100) {
        spawn_angle = entity_obj->angle + ((float)rand() / RAND_MAX * 2 * PI);
        spawn_angle = fmod(spawn_angle + 2*PI, 2*PI); // normalize

        diff = fabs(fmod(spawn_angle-entity_obj->angle + 3*PI, 2*PI) - PI);

        // reject engles withing 45 of players view
        if (diff > PI/4) { 
            found = 1;
        }

        count++;
    }

    float spawn_x = entity_obj->position_x + SPAWN_DIST * cosf(spawn_angle);
    float spawn_y = entity_obj->position_y + SPAWN_DIST * sinf(spawn_angle);

    *angle = spawn_angle;
    *x = spawn_x;
    *y = spawn_y;


}

int spawn(entety_t **entity, EntetyType type, entety_t *entity_obj){
    entety_t *obj = malloc(sizeof(entety_t));
    if (obj == NULL) {
        return 1;
    }

    float spawn_angle;
    float spawn_x;
    float spawn_y;

    if (entity_obj != NULL) {
        angle_position_calc(&spawn_angle, &spawn_x, &spawn_y, entity_obj);
    }

    switch (type) {
        case PLAYER:
            obj->entety_type = type;
            obj->position_x = 0.0;
            obj->position_y = 0.0;
            obj->angle = 0.0;
            obj->moving_speed = 5.5;
            obj->rotating_speed = 2.5;
            obj->lives = 3;
            obj->score = 0;
            break;
        case NOTRMAL_TANK:
            obj->entety_type = type;
            obj->position_x = spawn_x;
            obj->position_y = spawn_y;
            obj->angle = spawn_angle;
            obj->moving_speed = 3.0;
            obj->rotating_speed = 2.0;
            obj->lives = 1;
            obj->score = 500;
            break;
        case SUPER_TANK:
            obj->entety_type = type;
            obj->position_x = spawn_x;
            obj->position_y = spawn_y;
            obj->angle = spawn_angle; 
            obj->moving_speed = 6.5;
            obj->rotating_speed = 3.0;
            obj->lives = 1;
            obj->score = 1500;
            break;
        case UFO:
            obj->entety_type = type;
            obj->position_x = spawn_x;
            obj->position_y = spawn_y;
            obj->angle = spawn_angle;
            obj->moving_speed = 8.0;
            obj->rotating_speed = 10.0;
            obj->lives = 1;
            obj->score = 5000;
            break;
        case BULLET:
            obj->entety_type = type;
            obj->position_x = entity_obj->position_x + 30.0 * cosf(entity_obj->angle); // fix
            obj->position_y = entity_obj->position_y + 30.0 * sinf(entity_obj->angle); // fix
            obj->angle = entity_obj->angle; // fix
            obj->moving_speed = 15.0;
            obj->rotating_speed = 0;
            obj->lives = 1;
            obj->score = 0;
            break;
        case MISSILE:
            obj->entety_type = type;
            obj->position_x = spawn_x;
            obj->position_y = spawn_y;
            obj->angle = spawn_angle;
            obj->moving_speed = 25.0;
            obj->rotating_speed = 0.0;
            obj->lives = 1;
            obj->score = 1000;
            break;
        default:
            return 1;
    }

    *entity = obj;
    return 0;
}

void move(entety_t *entity, float dt, float np){
    // np - negative or positive (move forward or backwards)
    float dx = entity->moving_speed * dt * cosf(entity->angle) * np;
    float dy = entity->moving_speed * dt * sinf(entity->angle) * np;
    entity->position_x += dx;
    entity->position_y += dy;
}

void rotate(entety_t *entity, float dt, float np){
    // np - negative or positive (move forward or backwards)
    const float PI = 3.1415;
    float da = entity->rotating_speed * dt * np;
    entity->angle += da;
    entity->angle = fmodf(entity->angle + 2*PI, 2*PI);
}


// int destroy(entety_t *player, entety_t *bullet1, entety_t* bullet2, entety_t *enemy){
//     return 0;
// }