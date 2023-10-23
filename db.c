#include "header.h"

const char *DB_PATH = "../data/database.db";

enum RESULT initDb(void) {
    sqlite3 *db;
    char *err_msg = 0;

    int rc = sqlite3_open(DB_PATH, &db);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return ERROR;
    }


    char *sql = "CREATE TABLE IF NOT EXISTS users"
                "(id INTEGER PRIMARY KEY AUTOINCREMENT, "
                "name TEXT NOT NULL, "
                "password TEXT NOT NULL);"
                "DROP TABLE IF EXISTS records;"
                "CREATE TABLE IF NOT EXISTS records"
                "(id INTEGER PRIMARY KEY AUTOINCREMENT, "
                "user_id INTEGER, "
                "date TEXT, "
                "country TEXT, "
                "phone_num INTEGER, "
                "balance REAL, "
                "type INTEGER, "
                "FOREIGN KEY (user_id) REFERENCES users(id));";

    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    sqlite3_close(db);
    return 0;
}

sqlite3* openDb(void) {
    sqlite3 *db;
    int rc = sqlite3_open(DB_PATH, &db);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }
    return db;
}
