#include <stdio.h>
#include <time.h>
#include "db/scores.h"
    
// char *name = "Johnny_S";
// int score = 1000;
// scores_t *scores;
// DbExecCode db_status = handle_scores(name, score, &scores);
// if (db_status == DB_NAME_EXISTS) {
//     printf("Name already taken.\n");
//     return 1;
// } else if (db_status != DB_OK) {
//     printf("DB operation faild\n.");
//     return 1;
// }
// printf("TOP 10:\n");
// printf("PLACE | NAME | SCORE | TIME\n");
// for (int i = 0; i < scores->count; i++) {
//     score_t score = *(scores->data[i]);
//     time_t timestamp = score.timestamp;
//     printf("%i | %s | %i | %s", i+1, score.name, score.score, ctime(&timestamp));
// }
// free_scores(scores);