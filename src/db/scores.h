#pragma once

typedef enum {
    DB_OK,
    DB_NAME_EXISTS,
    DB_NAME_CHECK_FAIL,
    DB_OPEN_FAIL,
    DB_INSERT_FAIL,
    DB_SELECT_FAIL
} DbExecCode;

typedef struct Score {
    int id;
    char *name;
    int score;
    int timestamp;
} score_t;

typedef struct Scores {
    score_t **data;
    int count;
} scores_t;


DbExecCode handle_scores(char *name, int score, scores_t **scores);
void free_scores(scores_t *scores_list);