#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <sqlite3.h>

enum RESULT {
    OK,
    ERROR,
    CONTINUE
};

struct Date
{
    int month, day, year;
};

// all fields for each record of an account
struct Record
{
    int id;
    int userId;
    char name[100];
    char country[100];
    int phone;
    char accountType[10];
    int accountNbr;
    double amount;
    struct Date deposit;
    struct Date withdraw;
};

struct User
{
    int id;
    char name[50];
    char password[50];
};

// menu functions
enum RESULT mainMenu(struct User u, sqlite3* db);

// utils
void goMainMenu(struct User u, sqlite3 *db);
void success(struct User u, sqlite3 *db);
void displayAllAccounts(struct User u, sqlite3 *db);


// authentication functions
enum RESULT loginMenu(sqlite3 *db, struct User *u);
enum RESULT registerMenu(sqlite3 *db);
void validatePassword(struct User u);
enum RESULT confirmation(void);

// system function
void createNewAcc(struct User u, sqlite3 *db);
void checkExistingAccounts(struct User u, sqlite3 *db);
void checkAllAccounts(struct User u, sqlite3 *db);
void removeExistingAccount(struct User u, sqlite3 *db);


// update records
void updateAccount(struct User u, sqlite3 *db);
enum RESULT updatePhoneNum(int id, sqlite3 *db);
enum RESULT updateCountry(int id, sqlite3 *db);
void makeTransaction(struct User u, sqlite3 *db);
void transferOwner(struct User u, sqlite3 *db);

// Database

enum RESULT initDb(void);
sqlite3* openDb(void);

