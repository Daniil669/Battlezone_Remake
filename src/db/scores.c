#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <time.h>
#include "scores.h"


static DbExecCode open_db(sqlite3 **db) {
    int rc;
    char *errMsg = NULL;
    char *sql = "CREATE TABLE IF NOT EXISTS highscores(" \
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"\
        "name TEXT NOT NULL,"\
        "score INT NOT NULL,"\
        "timestamp INT NOT NULL);";


    rc = sqlite3_open("./data/highscores.db", db);
    if (rc != SQLITE_OK) {
        printf("Error open DB\n");
        printf("%s\n", sqlite3_errmsg(*db));
        return DB_OPEN_FAIL;
    }

    rc = sqlite3_exec(*db, sql, NULL, NULL, &errMsg);
    if (rc != SQLITE_OK) {
        printf("SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
        return DB_OPEN_FAIL;
    }

    return DB_OK;
}

static int set_rows_count(void *data, int colmncount, char **fields_row, char **colmnnames) {
    (void)colmncount;
    (void)colmnnames;
    *((int *)data) = atoi(fields_row[0]);
    return 0;
}


static int callback(void *data, int colmncount, char **fields_row, char **colmnnames) {
    (void)colmncount;
    (void)colmnnames;
    score_t *score = malloc(sizeof(score_t));
    if (score == NULL) {
        return 1;
    }

    char *name_str = malloc((strlen(fields_row[1]) + 1) * sizeof(char));
    if (name_str == NULL) {
        free(score);
        return 1;
    }

    score->id = atoi(fields_row[0]);
    strcpy(name_str, fields_row[1]);
    score->name = name_str;
    score->score = atoi(fields_row[2]);
    score->timestamp = atoi(fields_row[3]);

    ((scores_t *)data)->data[((scores_t *)data)->count] = score;
    ((scores_t *)data)->count++;

    return 0;
}


static DbExecCode get_data_db(sqlite3 **db, scores_t **scores) {
    int rc;
    char *errMsg = NULL;
    char *sql;

    int rows_count = 0;
    sql = "SELECT COUNT(*) FROM highscores";
    rc = sqlite3_exec(*db, sql, set_rows_count, (void *)&rows_count, &errMsg);
    if (rc != SQLITE_OK) {
        printf("SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
        return DB_SELECT_FAIL;
    }

    if (rows_count == 11) {
        sql = "DELETE FROM highscores WHERE id = (SELECT id FROM highscores ORDER BY score ASC, timestamp ASC LIMIT 1)";
            rc = sqlite3_exec(*db, sql, NULL, NULL, &errMsg);
        if (rc != SQLITE_OK && rc != SQLITE_DONE) {
            printf("SQL error: %s\n", errMsg);
            sqlite3_free(errMsg);
            return DB_SELECT_FAIL;
        }
    }


    sql = "SELECT * FROM highscores ORDER BY score DESC, timestamp DESC";
    rc = sqlite3_exec(*db, sql, callback, (void *)*scores, &errMsg);
    if (rc != SQLITE_OK) {
        printf("SQL error: %s\n", errMsg);
        sqlite3_free(errMsg); 
        return DB_SELECT_FAIL;
    }

    return DB_OK;
}


static DbExecCode add_data_db(sqlite3 **db, char *name, int score) {
    int rc;
    time_t curtime;
    char *sql = "INSERT INTO highscores (name, score, timestamp) VALUES (?, ?, ?);";

    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(*db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        printf("SQL error: %s\n", sqlite3_errmsg(*db));
        return DB_INSERT_FAIL;
    }


    time(&curtime);

    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, score);
    sqlite3_bind_int64(stmt, 3, (sqlite3_int64)curtime);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        printf("SQL error: %s\n", sqlite3_errmsg(*db));
        return DB_INSERT_FAIL;
    }

    sqlite3_finalize(stmt);

    return DB_OK;
}


static DbExecCode check_name(sqlite3 **db, char *name) {
    int rc;
    char *sql = "SELECT id FROM highscores WHERE name = ?";
    int id = -1;
    sqlite3_stmt *stmt;

    rc = sqlite3_prepare_v2(*db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        printf("SQL error: %s\n", sqlite3_errmsg(*db));
        return DB_NAME_CHECK_FAIL;
    }

    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        id = sqlite3_column_int(stmt, 0);
    } else if (rc != SQLITE_DONE) {
        printf("SQL error: %s\n", sqlite3_errmsg(*db));
        return DB_NAME_CHECK_FAIL;
    }  

    sqlite3_finalize(stmt);

    if (id != -1) {
        return DB_NAME_EXISTS;
    }

    return DB_OK;
}


DbExecCode handle_scores(char *name, int score, scores_t **scores) {
    sqlite3 *db;

    if (open_db(&db) != DB_OK) {
        return DB_OPEN_FAIL;
    }

    DbExecCode name_check = check_name(&db, name);
    if ( name_check == DB_NAME_EXISTS) {
        return DB_NAME_EXISTS;
    } else if (name_check != DB_OK) {
        return DB_NAME_CHECK_FAIL;
    }

    if (add_data_db(&db, name, score) != DB_OK) {
        return  DB_INSERT_FAIL;
    }

    score_t **data = calloc(11, sizeof(score_t *));
    scores_t *scores_list = malloc(sizeof(scores_t));

    scores_list->count = 0;
    scores_list->data = data;

    if (get_data_db(&db, &scores_list) != DB_OK){
        free(data);
        free(scores_list);
        return DB_SELECT_FAIL;
    }

    *scores = scores_list;

    sqlite3_close(db);

    return DB_OK;
}

void free_scores(scores_t *scores_list) {
    for (int i = 0; i < scores_list->count; i++) {
        free(scores_list->data[i]->name);
        free(scores_list->data[i]);
    }
    free(scores_list->data);
    free(scores_list);
}