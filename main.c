#include "header.h"

enum RESULT mainMenu(struct User u, sqlite3 *db) {
    int option;
    system("clear");
    printf("\n\n\t\t======= ATM =======\n\n");
    printf("\n\t\t-->> Feel free to choose one of the options below <<--\n");
    printf("\n\t\t[1]- Create a new account\n");
    printf("\n\t\t[2]- Update account information\n");
    printf("\n\t\t[3]- Check accounts\n");
    printf("\n\t\t[4]- Check list of owned account\n");
    printf("\n\t\t[5]- Make Transaction\n");
    printf("\n\t\t[6]- Remove existing account\n");
    printf("\n\t\t[7]- Transfer ownership\n");
    printf("\n\t\t[8]- Exit\n");
    scanf("%d", &option);

    switch (option)
    {
    case 1:
        createNewAcc(u, db);
        break;
    case 2:
        updateAccount(u, db);
        break;
    case 3:
        checkExistingAccounts(u, db);
        break;
    case 4:
        checkAllAccounts(u, db);
        break;
    case 5:
        makeTransaction(u, db);
        break;
    case 6:
        removeExistingAccount(u, db);
        break;
    case 7:
        transferOwner(u, db);
        break;
    case 8:
        exit(1);
    default:
        printf("Invalid operation!\n");
    }
    return OK;
}

void initMenu(struct User *u, sqlite3 *db)
{
    enum RESULT result;
    int option;
    system("clear");

    do {
        printf("\n\n\t\t======= ATM =======\n");
        printf("\n\t\t-->> Feel free to login / register :\n");
        printf("\n\t\t[1]- login\n");
        printf("\n\t\t[2]- register\n");
        printf("\n\t\t[3]- exit\n");
        scanf("%d", &option);

        switch (option)
        {
            case 1:
                result = loginMenu(db, u);
                break;
            case 2:
                result = registerMenu(db);
                break;
            case 3:
                exit(1);
            default:
                printf("Insert a valid operation!\n");
        }

        if (result == ERROR) {
            printf("Unexpected error. Exit program.\n");
            exit(1);
        }
    } while (result == CONTINUE);
}

int main(void) {
    int result = initDb();
    if (result == OK) {
        printf("Database created successfully.\n");
    } else {
        printf("Failed to create database.\n");
        return ERROR;
    }

    struct User u;
    sqlite3 *db = openDb();

    initMenu(&u, db);
    mainMenu(u, db);
    return OK;
}
