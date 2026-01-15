#include <stdio.h>
#include <string.h>
#include <sqlite3.h>

int main() {
    sqlite3 *DB;

    int rc = sqlite3_open("./data/highscores.db", &DB);
    if (rc != SQLITE_OK) {
        printf("Error open DB\n");
        printf("%s\n", sqlite3_errmsg(DB));
    } else {
        printf("Connected!\n");
    }
    sqlite3_close(DB);
    printf("Connection closed.\n");
    return 0;
}