#include "header.h"

enum RESULT updatePhoneNum(int id, sqlite3 *db) {
    const char *tail;
    sqlite3_stmt *stmt;
    int phone_num;
    printf("\t Enter new phone number: ");
    scanf("%d", &phone_num);

    char *update = "UPDATE RECORDS SET phone_num = ? WHERE id = ?";

    int rc = sqlite3_prepare_v2(db, update, -1, &stmt, &tail);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return ERROR;
    }

    sqlite3_bind_int(stmt, 1, phone_num);
    sqlite3_bind_int(stmt, 2, id);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Cannot execute statement: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return ERROR;
    }

    rc = sqlite3_finalize(stmt);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot finalize statement: %s\n", sqlite3_errmsg(db));
        return ERROR;
    } else {
        return CONTINUE;
    }
}

enum RESULT updateCountry(int id, sqlite3 *db) {
    const char *tail;
    sqlite3_stmt *stmt;
    char country[100];

    printf("\t Enter new country: ");
    scanf("%s", country);

    char *update = "UPDATE RECORDS SET country = ? WHERE id = ?";
    int rc = sqlite3_prepare_v2(db, update, -1, &stmt, &tail);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return ERROR;
    }

    sqlite3_bind_text(stmt, 1, country, -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, id);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Cannot execute statement: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return ERROR;
    }

    rc = sqlite3_finalize(stmt);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot finalize statement: %s\n", sqlite3_errmsg(db));
        return ERROR;
    } else {
        return CONTINUE;
    }
}

void deposit(struct Record r, sqlite3 *db);
void withdraw(struct Record r, sqlite3 *db);

void makeTransaction(struct User u, sqlite3 *db) {
    system("clear");
    displayAllAccounts(u, db);

    const char *tail;
    sqlite3_stmt *stmt;
    struct Record r;

    while(1) {
        scanf("%d", &r.id);
        char *select = "SELECT type, balance FROM records WHERE id = ? AND user_id = ?";
        int rc = sqlite3_prepare_v2(db, select, -1, &stmt, &tail);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Cannot prepare statement: %s\n", sqlite3_errmsg(db));
            sqlite3_close(db);
            return;
        }

        sqlite3_bind_int(stmt, 1, r.id);
        sqlite3_bind_int(stmt, 2, u.id);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            const unsigned char* type = sqlite3_column_text(stmt, 1);
            double account_balance = sqlite3_column_double(stmt, 2);
            if (strcmp((const char*) type, "savings") || strcmp((const char*) type, "current")) {
                r.amount = account_balance;
                break;
            } else {
                printf("Not allowed to do transactions for account type: %s\n", type);
            }
        } else {
            printf("Invalid account.\n");
            return;
        }

    }

    printf("\t Make transaction\n");
    printf("\t [1] Deposit\n");
    printf("\t [2] Withdraw\n");
    printf("\t [0] Exit\n");
    int selection = -1;

    while (selection != 0) {
        scanf("%d", &selection);
        switch (selection) {
            case 1:
                deposit(r, db);
                break;
            case 2:
                withdraw(r, db);
                break;
            case 0:
                goMainMenu(u, db);
                break;
            default:
                printf("Invalid option.\n");
        }
    }
}

void deposit(struct Record r, sqlite3 *db) {
    const char *tail;
    sqlite3_stmt *stmt;

    double depositAmount = 0;
    printf("Current balance: %lf\n", r.amount);
    printf("Select amount to deposit: ");
    while (depositAmount <= 0) {
        scanf("%lf", &depositAmount);
        if (depositAmount <= 0) {
            printf("ERROR: Cannot deposit negative or empty amount.\n");
        }
    }

    r.amount += depositAmount;
    printf("Funds have been deposited! New balance: %lf\n", r.amount);

    char *update = "UPDATE records SET amount = ? WHERE id = ?";

    int rc = sqlite3_prepare_v2(db, update, -1, &stmt, &tail);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    sqlite3_bind_double(stmt, 1, r.amount);
    sqlite3_bind_int(stmt, 2, r.id);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Cannot execute statement: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return;
    }

    rc = sqlite3_finalize(stmt);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot finalize statement: %s\n", sqlite3_errmsg(db));
        return;
    }
}

void withdraw(struct Record r, sqlite3 *db) {
    if (r.amount == 0.0) {
        printf("Account is empty.\n");
        return;
    }
    const char *tail;
    sqlite3_stmt *stmt;

    double withdrawAmount = 0.0;
    printf("Current balance: %lf\n", r.amount);
    printf("Select amount to deposit: ");
    while (withdrawAmount <= 0.0) {
        scanf("%lf", &withdrawAmount);
        if (withdrawAmount <= 0.0) {
            printf("ERROR: Cannot withdraw negative or empty amount.\n");
        }

        if (withdrawAmount > r.amount) {
            printf("ERROR: Cannot withdraw more than your current balance.\n");
            withdrawAmount = 0.0;
        }
    }

    r.amount -= withdrawAmount;
    printf("Funds have been withdrawn! New balance: %lf\n", r.amount);

    char *update = "UPDATE records SET amount = ? WHERE id = ?";

    int rc = sqlite3_prepare_v2(db, update, -1, &stmt, &tail);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    sqlite3_bind_double(stmt, 1, r.amount);
    sqlite3_bind_int(stmt, 2, r.id);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Cannot execute statement: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return;
    }

    rc = sqlite3_finalize(stmt);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot finalize statement: %s\n", sqlite3_errmsg(db));
        return;
    }
}

void removeExistingAccount(struct User u, sqlite3 *db) {
    system("clear");
    printf("REMOVE ACCOUNT\n");
    displayAllAccounts(u, db);

    int accountId;
    printf("Select which account id you want to remove: ");
    scanf("%d", &accountId);

    validatePassword(u);
    if (confirmation() == ERROR) {
        goMainMenu(u, db);
        return;
    }


    char *delete = "DELETE FROM RECORDS WHERE id = ? AND user_id = ?";
    sqlite3_stmt *stmt;

    int rc = sqlite3_prepare_v2(db, delete, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare statement: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_int(stmt, 1, accountId);
    sqlite3_bind_int(stmt, 2, u.id);
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Cannot execute statement: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return;
    } else {
        printf("Account successfully deleted. \n");
    }

    rc = sqlite3_finalize(stmt);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot finalize statement: %s\n", sqlite3_errmsg(db));
        return;
    }
    goMainMenu(u, db);
}

void transferOwner(struct User u, sqlite3 *db) {
    printf("TRANSFER ACCOUNT\n");
    displayAllAccounts(u, db);

    int transferFromId;
    printf("Select which id you want to transfer: ");
    scanf("%d", &transferFromId);

    char *select = "SELECT id, name FROM users WHERE id != ?";
    sqlite3_stmt *stmt;

    int rc = sqlite3_prepare_v2(db, select, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare statement: %s\n", sqlite3_errmsg(db));
        return;
    }

    int max_id;
    sqlite3_bind_int(stmt, 1, u.id);
    printf("Select which id you want to transfer to: \n");
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int user_id = sqlite3_column_int(stmt, 1);
        const unsigned char* name = sqlite3_column_text(stmt, 2);
        printf("Id: %d, name: %s\n", user_id, name);
        max_id = user_id;
    }

    int transferToId;
    while (1) {
        scanf("%d", &transferToId);
        if (transferToId != u.id && transferToId > 0 && transferToId <= max_id) {
            break;
        } else {
            printf("Invalid id.\n");
        }
    }


    validatePassword(u);
    if (confirmation() == ERROR) {
        goMainMenu(u, db);
        return;
    }

    char *update = "UPDATE records SET user_id = ? WHERE id = ?";
    rc = sqlite3_prepare_v2(db, update, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare statement: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_int(stmt, 1, transferToId);
    sqlite3_bind_int(stmt, 2, transferFromId);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Cannot execute statement: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return;
    }

    rc = sqlite3_finalize(stmt);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot finalize statement: %s\n", sqlite3_errmsg(db));
        return;
    } else {
        printf("Account successfully transferred.\n");
    }
    goMainMenu(u, db);
}
