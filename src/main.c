#include <stdio.h>
#include <time.h>
#include "db/scores.h"

int main() {
    char *name = "Tommy";
    int score = 1000;
    scores_t *scores;
    if (handle_scores(name, score, &scores)) {
        printf("DB FAIL");
        return 1;
    }
    printf("TOP 10:\n");
    printf("PLACE | NAME | SCORE | TIME\n");
    for (int i = 0; i < scores->count; i++) {
        score_t score = *(scores->data[i]);
        time_t timestamp = score.timestamp;
        printf("%i | %s | %i | %s\n", i+1, score.name, score.score, ctime(&timestamp));
    }
    free_scores(scores);
    return 0;
}