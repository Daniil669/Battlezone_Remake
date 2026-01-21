#pragma once

typedef enum {
    PLAYER,
    NOTRMAL_TANK,
    SUPER_TANK,
    UFO,
    BULLET,
    MISSILE
} EntetyType;


typedef struct Entety {
    EntetyType entety_type;

    float position_x;
    float position_y;
    float angle;

    float moving_speed;
    float rotating_speed;

    int lives;
    int score;

} entety_t;

int spawn(entety_t **entety, EntetyType type, entety_t *entety_obj);
void move(entety_t **entety);
void rotate(entety_t **entety);
void shoot(entety_t **entety);
int destroy(entety_t **entety);

// typedef struct Player {
// } player_t;

// typedef struct Enemy {
// } enemy_t;

// typedef struct Bullet {
// } bullet_t;

// typedef struct Missile {
// } missile_t;