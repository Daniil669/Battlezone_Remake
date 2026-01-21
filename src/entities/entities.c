#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "entities.h"

static void angle_position_calc(float *angle, float *x, float *y, entety_t *entety_obj) {
    const float PI = 3.1415;
    // const int FOV = 90; // 90 deg field of view
    const float SPAWN_DIST = 1200;

    float spawn_angle;
    float diff;

    srand(time(NULL));

    for (int i = 0; i < 10; i++) {
        spawn_angle = entety_obj->angle + (rand() % 360) * 2 * PI;
        spawn_angle = fmod(spawn_angle + 2 * PI, 2 * PI);    // normalize

        diff = fabs(fmod(spawn_angle - entety_obj->angle + 3 * PI, 2 * PI) - PI);

        if (diff <= PI/4) { 
            continue;
        }

        break;
    }

    float spawn_x = entety_obj->position_x + SPAWN_DIST * cos(spawn_angle);
    float spawn_y = entety_obj->position_y + SPAWN_DIST * sin(spawn_angle);

    *angle = spawn_angle;
    *x = spawn_x;
    *y = spawn_y;


}

int spawn(entety_t **entety, EntetyType type, entety_t *entety_obj){
    entety_t *obj = malloc(sizeof(entety_t));
    if (obj == NULL) {
        return 1;
    }

    float spawn_angel;
    float spawn_x;
    float spawn_y;

    if (entety_obj != NULL) {
        angle_position_calc(&spawn_angel, &spawn_x, &spawn_y, entety_obj);
    }

    switch (type) {
        case PLAYER:
            obj->entety_type = type;
            obj->position_x = 0;
            obj->position_y = 0;
            obj->angle = 0;
            obj->moving_speed = 5.5;
            obj->rotating_speed = 2.5;
            obj->lives = 3;
            obj->score = 0;
            break;
        case NOTRMAL_TANK:
            obj->entety_type = type;
            obj->position_x = spawn_x;
            obj->position_y = spawn_y;
            obj->angle = spawn_angel;
            obj->moving_speed = 3.0;
            obj->rotating_speed = 2.0;
            obj->lives = 1;
            obj->score = 500;
            break;
        case SUPER_TANK:
            obj->entety_type = type;
            obj->position_x = spawn_x;
            obj->position_y = spawn_y;
            obj->angle = spawn_angel; 
            obj->moving_speed = 6.5;
            obj->rotating_speed = 3.0;
            obj->lives = 1;
            obj->score = 1500;
        case UFO:
            obj->entety_type = type;
            obj->position_x = spawn_x;
            obj->position_y = spawn_y;
            obj->angle = spawn_angel;
            obj->moving_speed = 8.0;
            obj->rotating_speed = 10.0;
            obj->lives = 1;
            obj->score = 5000;
        case BULLET:
            obj->entety_type = type;
            obj->position_x = entety_obj->position_x;
            obj->position_y = entety_obj->position_y;
            obj->angle = entety_obj->angle;
            obj->moving_speed = 15.0;
            obj->rotating_speed = 0;
            obj->lives = 1;
            obj->score = 0;
        case MISSILE:
            obj->entety_type = type;
            obj->position_x = spawn_x;
            obj->position_y = spawn_y;
            obj->angle = spawn_angel;
            obj->moving_speed = 25.0;
            obj->rotating_speed = 0;
            obj->lives = 1;
            obj->score = 1000;
        default:
            return 1;
    }

    *entety = obj;
    return 0;
}

void move(entety_t **entety){

}

void rotate(entety_t **entety){

}

void shoot(entety_t **entety){

}

int destroy(entety_t **entety){

}