#include "header.h"

enum RESULT loginMenu(sqlite3 *db, struct User *u) {
    system("clear");
    char username[50];
    char password[50];

    printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\t User Login:");
    scanf("%s", username);

    printf("\n\n\n\n\n\t\t\t\tEnter the password to login:");
    scanf("%s", password);

    const char* sql = "SELECT username, password FROM users WHERE username = ? AND password = ?";
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return ERROR;
    }

    rc = sqlite3_bind_text(stmt, 1, username, -1, SQLITE_TRANSIENT);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot bind username: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return ERROR;
    }

    rc = sqlite3_bind_text(stmt, 2, password, -1, SQLITE_TRANSIENT);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot bind password: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return ERROR;
    }

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        u->id = sqlite3_column_int(stmt, 0);
        const unsigned char* username_str = sqlite3_column_text(stmt, 1);
        const unsigned char* password_str = sqlite3_column_text(stmt, 2);
        sprintf(u->name, "%s", username_str);
        sprintf(u->password, "%s", password_str);
        // printf("Username and password combination exists in the users table.\n");
    } else if (rc == SQLITE_DONE) {
        sqlite3_close(db);
        return CONTINUE;
        // printf("Username and password combination does not exist in the users table.\n");
    } else {
        fprintf(stderr, "Cannot execute statement: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return ERROR;
    }

    rc = sqlite3_finalize(stmt);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot finalize statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return ERROR;
    }

    return OK;
}

enum RESULT registerMenu(sqlite3 *db) {
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

    const char *tail;
    sqlite3_stmt *stmt;

    const char *select = "SELECT id FROM users WHERE name = ?;";
    int rc = sqlite3_prepare_v2(db, select, -1, &stmt, &tail);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return ERROR;
    }

    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);

    if (rc == SQLITE_ROW) {
        fprintf(stderr, "User already exists.\n");
        sqlite3_finalize(stmt);
        return CONTINUE;
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
        return ERROR;
    }

    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, password, -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);

    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Failed to insert data: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return ERROR;
    }

    sqlite3_finalize(stmt);
    return CONTINUE;
}

void validatePassword(struct User u) {
    char pw[50];
    while (1) {
        printf("\nEnter password: ");
        scanf("%s", pw);
        if (!strcmp(pw, u.password)) {
            printf("Password is incorrect. Please try again.\n");
        } else {
            return;
        }
    }
}

enum RESULT confirmation(void) {
    char selection;
    printf("WARNING! This process is final and can't be undone. Any remaining funds will be removed.\n");
    printf("Are you sure you want to proceed? y/n\n");
    scanf("%c", &selection);

    if (selection != 'y' && selection != 'Y') {
        printf("Aborting.\n");
        return ERROR;
    }
    return OK;
}
