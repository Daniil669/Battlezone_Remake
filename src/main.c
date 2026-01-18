#include <stdio.h>
#include <time.h>
#include "scores.h"

int main() {
    char *name = "Johnny_S";
    int score = 69000;
    scores_t *scores;
    if (handle_scores(name, score, &scores)) {
        printf("DB FAIL");
        return 1;
    }
    printf("TOP 10:\n");
    printf("PLACE | NAME | SCORE | TIME\n");
    for (int i = 0; i < scores->count; i++) {
        score_t score = *(scores->data[i]);
        printf("%i | %s | %i | %s\n", i+1, score.name, score.score, ctime((time_t)score.timestamp));
    }
    return 0;
}