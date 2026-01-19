#pragma once

// typedef enum DB_exec_codes {
//     DB_OK = 0,
// };

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


int handle_scores(char *name, int score, scores_t **scores);
void free_scores(scores_t *scores_list);