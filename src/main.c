#include <stdio.h>
#include <time.h>
#include "scores.h"

int main() {
    char *name = "Johnny";
    int score = 69000;
    scores_t **scores = handle_score(name, score);
    printf("TOP 10:\n");
    printf("PLACE | NAME | SCORE | TIME\n");
    for (int i = 0; i < (*scores)->count; i++) {
        score_t score = (*scores)->data[i];
        printf("%i | %s | %i | %s\n", i+1, score.name, score.score, ctime((time_t)score.timestamp));
    }
    return 0;
}