#include "header.h"

void saveAccount(struct Record r, struct Date d, sqlite3* db);

void createNewAcc(struct User u, sqlite3* db) {
    struct Record r;
    struct Date d;

    system("clear");
    printf("\t\t\t===== New record =====\n");

    printf("\nEnter today's date(mm/dd/yyyy):");
    scanf("%d/%d/%d", &d.month, &d.day, &d.year);

    printf("\nEnter the account number:");
    scanf("%d", &r.accountNbr);

    printf("\nEnter the country:");
    scanf("%s", &r.country[100]);

    printf("\nEnter the phone number:");
    scanf("%d", &r.phone);

    printf("\nEnter amount to deposit: $");
    scanf("%lf", &r.amount);

    printf("\nChoose the type of account:\n\t"
           "-> saving\n\t"
           "-> current\n\t"
           "-> fixed01(for 1 year)\n\t"
           "-> fixed02(for 2 years)\n\t"
           "-> fixed03(for 3 years)\n\n\t"
           "Enter your choice:");
    while (1 == 1) {
        scanf("%s", r.accountType);
        if (strcmp(r.accountType, "saving")) {
            break;
        }
        if (strcmp(r.accountType, "current")) {
            break;
        }
        if (strcmp(r.accountType, "fixed01")) {
            break;
        }
        if (strcmp(r.accountType, "fixed02")) {
            break;
        }
        if (strcmp(r.accountType, "fixed03")) {
            break;
        }

        printf("Invalid account type. \n");
    }

    r.userId = u.id;

    saveAccount(r, d, db);
    success(u, db);
}

void updateAccount(struct User u, sqlite3 *db) {
    system("clear");
    printf("\tUPDATE ACCOUNT\n");

    const char *tail;
    sqlite3_stmt *stmt;

    int result = CONTINUE;
    int id;

    while (result == CONTINUE) {
        printf("\t\tEnter id: ");
        scanf("%d", &id);
        int count = 0;
        char *select = "SELECT COUNT(*) FROM RECORDS WHERE id = ? AND user_id = ?";

        int rc = sqlite3_prepare_v2(db, select, -1, &stmt, &tail);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Cannot prepare statement: %s\n", sqlite3_errmsg(db));
            sqlite3_close(db);
            return;
        }

        sqlite3_bind_int(stmt, 1, u.id);
        sqlite3_bind_int(stmt, 2, id);

        rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW) {
            count = sqlite3_column_int(stmt, 0);
        }
        if (count > 0) {
            result = OK;
        } else {
            printf("Record with id %d does not exist.\n", id);
        }
    }

    system("clear");
    result = CONTINUE;

    while (result == CONTINUE) {
        printf("\t Select field to update:\n");
        printf("\t [1] Country\n");
        printf("\t [2] Phone number\n");
        printf("\t [0] Abort update\n");
        int selection = 0;
        scanf("%d", &selection);
        switch (selection) {
            case 0:
                success(u, db);
                return;
            case 1:
                result = updateCountry(u.id, db);
                break;
            case 2:
                result = updatePhoneNum(u.id, db);
                break;
            default:
                printf("\n Invalid selection.\n");
                break;
        }
    }

}

void saveAccount(struct Record r, struct Date d, sqlite3* db) {
    char formatDate[11];
    sprintf(formatDate, "%02d/%02d/%04d", d.day, d.month, d.year);

    const char *tail;
    sqlite3_stmt *stmt;

    char *insert = "INSERT INTO users VALUES(?, ?, ?, ?, ?, ?)";
    int rc = sqlite3_prepare_v2(db, insert, -1, &stmt, &tail);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }
    sqlite3_bind_int(stmt, 1, r.userId);
    sqlite3_bind_text(stmt, 2, formatDate, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, r.country, -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 4, r.phone);
    sqlite3_bind_double(stmt, 5, r.amount);
    sqlite3_bind_text(stmt, 6, r.accountType, -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);

    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Failed to insert data: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        exit(1);
    }

    sqlite3_finalize(stmt);
}



void checkAllAccounts(struct User u, sqlite3 *db)
{
    displayAllAccounts(u, db);
    success(u, db);
}

void checkExistingAccounts(struct User u, sqlite3 *db) {
    const char *tail;
    sqlite3_stmt *stmt;

    char *select = "SELECT id, type FROM RECORDS WHERE user_id = ?";
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
        const unsigned char* type = sqlite3_column_text(stmt, 2);
        printf("\tAccount id: %d, type: %s\n", records_id, type);
    }

    int id;
    while (1) {
        printf("\t0 to exit\n");
        printf("\tEnter id: ");
        scanf("%d",&id);

        if (id == 0){
            success(u, db);
            return;
        }

        select = "SELECT type FROM RECORDS WHERE id = ?";
        rc = sqlite3_prepare_v2(db, select, -1, &stmt, &tail);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Cannot prepare statement: %s\n", sqlite3_errmsg(db));
            sqlite3_close(db);
            return;
        }

        sqlite3_bind_int(stmt, 1, id);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            const unsigned char* type = sqlite3_column_text(stmt, 1);
            if (strcmp((const char*) type, "savings")) {
                printf("\t\taccount %d -> savings: interest rate 7%%\n", id);
            }
            if (strcmp((const char*) type, "fixed01")) {
                printf("\t\taccount %d -> fixed01(1 year account): interest rate 4%%\n", id);
            }
            if (strcmp((const char*) type, "fixed02")) {
                printf("\t\taccount %d -> fixed02(2 year account): interest rate 5%%\n", id);
            }
            if (strcmp((const char*) type, "fixed03")) {
                printf("\t\taccount %d -> fixed03(3 year account): interest rate 8%%\n", id);
            } else {
                printf("\t\taccount %d -> You will not get interests because the account is of type 'current'\n", id);
            }
        } else {
            printf("Invalid account.\n");
            exit(1);
        }
    }

}

