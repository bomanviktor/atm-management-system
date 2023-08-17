// Created by victor b on 2023-08-17.

#include "header.h"

int registration() {
    sqlite3 *db;
    sqlite3_stmt *stmt;
    const char *tail;

    system("clear");
    printf("\n\n\t\t======= REGISTER =======\n");

    char name[30];
    while (strlen(name) == 0) {
        printf("\n\t\tFull name: ");
        scanf("%s", name);

        if (strlen(name) == 0){
            printf("\n\t\tName is too short.\n");
        }
    }


    int rc = sqlite3_open(DB_PATH, &db);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    const char *select = "SELECT id FROM users WHERE name = ?;";
    rc = sqlite3_prepare_v2(db, select, -1, &stmt, &tail);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);

    if (rc == SQLITE_ROW) {
        fprintf(stderr, "User already exists.\n");
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 1;
    }



    char password[30];
    printf("\n\t\tPassword: ");
    scanf("%s", password);

    while (strlen(password) < 4) {
        printf("\n\t\tPassword (min 4 characters): ");
        scanf("%s", password);

        if (strlen(password) < 4){
            printf("\n\t\tPassword is too short.\n");
        }
    }

    char *insert = "INSERT INTO users VALUES(?, ?);";
    rc = sqlite3_prepare_v2(db, insert, -1, &stmt, &tail);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, password, -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);

    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Failed to insert data: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 1;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}