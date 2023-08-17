#include "header.h"

void mainMenu(struct User u) {

    sqlite3 *db;
    sqlite3_stmt *res;

    int rc = sqlite3_open(":memory:", &db);

    if (rc != SQLITE_OK) {

        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);

        return 1;
    }

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
        createNewAcc(u);
        break;
    case 2:
        updateAccount();
        // student TODO : add your **Update account information** function
        // here
        break;
    case 3:
        checkExistingAccounts();
        // student TODO : add your **Check the details of existing accounts** function
        // here
        break;
    case 4:
        checkAllAccounts(u);
        break;
    case 5:
        makeTransaction();
        // student TODO : add your **Make transaction** function
        // here
        break;
    case 6:
        removeExistingAccount();
        // student TODO : add your **Remove existing account** function
        // here
        break;
    case 7:
        transferOwner();
        // student TODO : add your **Transfer owner** function
        // here
        break;
    case 8:
        exit(1);
        break;
    default:
        printf("Invalid operation!\n");
    }
};

void initMenu(struct User *u)
{
    int r = 0;
    int option;
    system("clear");
    printf("\n\n\t\t======= ATM =======\n");
    printf("\n\t\t-->> Feel free to login / register :\n");
    printf("\n\t\t[1]- login\n");
    printf("\n\t\t[2]- register\n");
    printf("\n\t\t[3]- exit\n");
    while (!r)
    {
        scanf("%d", &option);
        switch (option)
        {
        case 1:
            loginMenu(u->name, u->password);
            if (strcmp(u->password, getPassword(*u)) == 0)
            {
                printf("\n\nPassword Match!");
            }
            else
            {
                printf("\nWrong password!! or User Name\n");
                exit(1);
            }
            r = 1;
            break;
        case 2:
            int result;
            result = registration();

            // student TODO : add your **Registration** function
            // here
            r = 1;
            break;
        case 3:
            exit(1);
            break;
        default:
            printf("Insert a valid operation!\n");
        }
    }
};




int init() {

    sqlite3 *db;
    char *err_msg = 0;

    int rc = sqlite3_open(DB_PATH, &db);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
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

int main() {
    int result = init();
    if (result == 0) {
        printf("Database created successfully.\n");
    } else {
        printf("Failed to create database.\n");
    }

    struct User u;
    
    initMenu(&u);
    mainMenu(u);

    return 0;
}
