#include "header.h"
void goMainMenu(struct User u, sqlite3 *db);
void displayAllAccounts(struct User u, sqlite3 *db);
void success(struct User u, sqlite3 *db);

void goMainMenu(struct User u, sqlite3 *db) {
    printf("\n[0] Go back to main menu.\n");
    printf("[1] Exit program.\n");
    int selection;
    while(1) {
        scanf("%d", &selection);
        switch (selection) {
            case 0:
                mainMenu(u ,db);
                break;
            case 1:
                printf("Program exiting.\n");
                sqlite3_close(db);
                exit(1);
            default:
                printf("Invalid option.\n");
        }
        if (selection == 1){
            break;
        }
    }
}

void displayAllAccounts(struct User u, sqlite3 *db) {
    const char *tail;
    sqlite3_stmt *stmt;

    char *select = "SELECT * FROM RECORDS WHERE user_id = ?";
    int rc = sqlite3_prepare_v2(db, select, -1, &stmt, &tail);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    rc = sqlite3_bind_int(stmt, 1, u.id);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot bind id: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }
    system("clear");
    printf("\t\t====== All accounts from user, %s =====\n\n", u.name);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int records_id = sqlite3_column_int(stmt, 1);
        int user_id = sqlite3_column_int(stmt, 2);
        const unsigned char* date = sqlite3_column_text(stmt, 3);
        const unsigned char* country = sqlite3_column_text(stmt, 4);
        int phone_num = sqlite3_column_int(stmt, 5);
        double balance = sqlite3_column_double(stmt, 6);
        int type = sqlite3_column_int(stmt, 7);
        const unsigned char* password_str = sqlite3_column_text(stmt, 8);
        printf("\tRecord ID: %d\n", records_id);
        printf("\tUser ID: %d\n", user_id);
        printf("\tDate: %s\n", date);
        printf("\tCountry: %s\n", country);
        printf("\tPhone Number: %d\n", phone_num);
        printf("\tBalance: %f\n", balance);
        printf("\tType: %d\n", type);
        printf("\tPassword: %s\n", password_str);
    }
    rc = sqlite3_finalize(stmt);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot finalize statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }
}

void success(struct User u, sqlite3 *db) {
    printf("\n✔ Success!\n\n");
    printf("Enter 1 to go to the main menu and 0 to exit!\n");
    system("clear");

    int option = 0;
    while (option != 1) {
        scanf("%d", &option);
        switch (option) {
            case 1:
                mainMenu(u, db);
                break;
            case 0:
                sqlite3_close(db);
                exit(1);
            default:
                printf("Insert a valid operation!\n");
                break;
        }
    }
}
