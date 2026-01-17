#pragma once

typedef struct Score {
    int id;
    char *name;
    int score;
    int timestamp;
} score_t;

typedef struct Scores {
    score_t *data;
    int count;
} scores_t;


score_t *handle_score(char *name, int score);