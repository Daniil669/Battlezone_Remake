#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <time.h>
#include "scores.h"


static int callback(void *data, int ncolumns, char **fields_row, char **column_names) {
    score_t *score = malloc(sizeof(score_t));
    char *name_str = malloc((strlen(fields_row[1]) + 1) * sizeof(char));

    score->id = (int)fields_row[0];
    strcpy(name_str, fields_row[1]);
    score->name = &name_str;
    score->score = (int)fields_row[2];
    score->timestamp = (int)fields_row[3];

    ((scores_t *)data)->data[((scores_t *)data)->count] = score;
    ((scores_t *)data)->count++;

    return 0;
}

static int open_db(sqlite3 **db) {
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
        printf("%s\n", sqlite3_errmsg(db));
        return 1;
    } else {
        printf("Connected!\n");
    }

    rc = sqlite3_exec(db, sql, NULL, NULL, &errMsg);
    if (rc != SQLITE_OK) {
        printf("SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
        return 1;
    } else {
        printf("Table 'highscores' created successfully.\n");
    }

    return 0;
}

static void set_rows_count(void *data, int ncolumns, char **fields_row, char **column_names) {
    *((int *)data) = (int)fields_row[0];
}

static void set_id(void *data, int ncolumns, char **fields_row, char **column_names) {
    *((int *)data) = (int)fields_row[0];
}

static int get_data_db(sqlite3 **db, scores_t **scores) {
    int rc;
    char *errMsg = NULL;
    char *sql;
    sqlite3_stmt *stmt;

    // check if 11 rows
    int rows_count = 0;
    sql = "SELECT COUNT(*) FROM highscores";
    rc = sqlite3_exec(db, sql, set_rows_count, &rows_count, &errMsg);
    if (rc != SQLITE_OK) {
        printf("SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
        return 1;
    } else {
        printf("Selected successfull.\n");
    }

    if (rows_count == 11) {
        // get id of the lowest and oldest; delete the lowest and oldest
        int id;
        sql = "DELETE FROM highscores WHERE id = (SELECT id FROM highscores ORDER BY score ASC, timestamp ASC LIMIT 1)";
            rc = sqlite3_exec(db, sql, set_id, &id, &errMsg);
        if (rc != SQLITE_OK) {
            printf("SQL error: %s\n", errMsg);
            sqlite3_free(errMsg);
            return 1;
        } else {
            printf("Selected successfull.\n");
            (*scores)->count--;
        }
    }


    // select the rest
    sql = "SELECT * FROM highscores ORDER BY score DESC";
    rc = sqlite3_exec(db, sql, callback, (void *)*scores, &errMsg);
    if (rc != SQLITE_OK) {
        printf("SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
        return 1;
    } else {
        printf("Selected successfull.\n");
    }
    return 0;
}


static int add_data_db(sqlite3 **db, char *name, int score) {
    int rc;
    char *errMsg = NULL;
    time_t curtime;
    char *sql = "INSERT INTO highscores (name, score, timestamp) VALUES (?, ?, ?);";

    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        printf("SQL error: %s\n", sqlite3_errmsg(db));
        return 1;
    }


    time(&curtime);

    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, score);
    sqlite3_bind_int64(stmt, 3, (sqlite3_int64)curtime);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        printf("SQL error: %s\n", sqlite3_errmsg(db));
        return 1;
    } else {
        sqlite3_finalize(stmt);
        printf("Inserted successfully.\n");
    }
    return 0;
}


static void delete_data_db(sqlite3 **db, int id) {
    int rc;
    char *errMsg = NULL;
    char *sql = "DELETE FROM highscores WHERE id = ?";
    sqlite3_stmt *stmt;

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        printf("SQL error: %s\n", sqlite3_errmsg(db));
    } else {
        printf("Prepared statement object has been returned.\n");
    }

    sqlite3_bind_int(stmt, 1, id);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        printf("SQL error: %s\n", sqlite3_errmsg(db));
    } else {
        printf("Deleted row with id: %i\n", id);
    }
}


static int check_name(sqlite3 **db, char *name) {
    int rc;
    char *errMsg = NULL;
    char *sql = "SELECT id FROM highscores WHERE name = ?";
    int id = -1;
    sqlite3_stmt *stmt;

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        printf("SQL error: %s\n", sqlite3_errmsg(db));
        return 1;
    } else {
        printf("Prepared statement object has been returned.\n");
    }

    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_OK) {
        printf("SQL error: %s\n", sqlite3_errmsg(db));
        return 1;
    } else {
        id = sqlite3_column_int(stmt, 0);
        printf("The name checked.\n");
        sqlite3_finalize(stmt);
    }

    if (id != -1) {
        return 2;
    }

    return 0;
}


int handle_scores(char *name, int score, scores_t **scores) {
    sqlite3 *db;

    if (open_db(&db)) {
        return 1;
    }

    // check if name already present in db
    if (check_name(db, name) == 2) {
        return 2;
    }

    // insert users data
    if (add_data_db(&db, name, score)) {
        return 1;
    }

    score_t **data = calloc(11, sizeof(score_t));
    scores_t *scores_list = malloc(sizeof(scores_t));

    scores_list->count = 0;
    scores_list->data = data;

    // select all objects
    if (get_data_db(&db, &scores_list)){
        free(data);
        free(scores_list);
        return 1;
    }

    *scores = scores_list;

    sqlite3_close(db);
    printf("Connection closed.\n");

    return 0;
}

void free_scores(scores_t *scores_list) {
    for (int i = 0; i < scores_list->count; i++) {
        free((*scores_list->data[i]->name));
        free(scores_list->data[i]);
    }
    free(scores_list);
}