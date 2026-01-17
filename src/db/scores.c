#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include <time.h>
#include "scores.h"


static int callback(void *data, int ncolumns, char **fields_row, char **column_names) {
    printf("%s\n", (const char *)data);
    for (int i = 0; i < ncolumns; i++) {
        printf("%s - %s\n", column_names[i], fields_row[i] ? fields_row[i] : "NULL");
    }
    printf("\n");
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
    } else {
        printf("Connected!\n");
    }

    rc = sqlite3_exec(db, sql, NULL, NULL, &errMsg);
    if (rc != SQLITE_OK) {
        printf("SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
    } else {
        printf("Table 'highscores' created successfully.\n");
    }

    return 0;
}

static void get_data_db(sqlite3 **db) {
    int rc;
    char *errMsg = NULL;
    char *sql = "SELECT * FROM highscores ORDER BY score DESC";
    const char *data = "Callback function called.";

    rc = sqlite3_exec(db, sql, callback, (void *)data, &errMsg);
    if (rc != SQLITE_OK) {
        printf("SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
    } else {
        printf("Selected successfull.\n");
    }
}


static void add_data_db(sqlite3 **db, char *name, int score, int timestamp) {
    int rc;
    char *errMsg = NULL;
    char *sql = "INSERT INTO highscores (name, score, timestamp) VALUES (?, ?, ?);";

    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        printf("SQL error: %s\n", sqlite3_errmsg(db));
    }

    time_t curtime;
    time(&curtime);

    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, score);
    sqlite3_bind_int64(stmt, 3, (sqlite3_int64)curtime);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        printf("SQL error: %s\n", sqlite3_errmsg(db));
    } else {
        sqlite3_finalize(stmt);
        printf("Inserted successfully.\n");
    }
}


static void update_data_db(sqlite3 **db, int id, char *name, int score, int timestamp) {
    int rc;
    char *errMsg = NULL;
    char *sql = "UPDATE highscores SET name = ?, score = ?, timestamp = ? WHERE id = ?";
    sqlite3_stmt *stmt;

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        printf("SQL error: %s\n", sqlite3_errmsg(db));
    } else {
        printf("Prepared statement object has been returned.\n");
    }

    time(&timestamp);

    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, score);
    sqlite3_bind_int64(stmt, 3, (sqlite3_int64)timestamp);
    sqlite3_bind_int(stmt, 4, id);

    rc = sqlite3_step(stmt);
       if (rc != SQLITE_OK) {
        printf("SQL error: %s\n", sqlite3_errmsg(db));
    } else {
        sqlite3_finalize(stmt);
        printf("Inserted successfully.\n");
    }
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


score_t *handle_score(char *name, int score) {
    sqlite3 *db;

    if (open_db(&db)) {
        return NULL;
    } 

    sqlite3_close(db);
    printf("Connection closed.\n");
}