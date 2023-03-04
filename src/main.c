#include "header.h"

int registerAcc();
int errorMessage();
void mainMenu(struct User u)
{
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
    printf("\n\t\t");
    scanf("%d", &option);
    switch (option)
    {
    case 1:
        createNewAcc(u);
        break;
    case 2:
        // student TODO : add your **Update account information** function
        // here
        break;
    case 3:
        // student TODO : add your **Check the details of existing accounts** function
        // here
        break;
    case 4:
        checkAllAccounts(u);
        break;
    case 5:
        // student TODO : add your **Make transaction** function
        // here
        break;
    case 6:
        // student TODO : add your **Remove existing account** function
        // here
        break;
    case 7:
        // student TODO : add your **Transfer owner** function
        // here
        break;
    case 8:
        exit(1);
        break;
    default:
        printf("Invalid operation!\n");
    }
}

void updateAccountInformation(struct User *u)
{

}

void initMenu(struct User *u)
{
    int r = 0;
    int option;
    while (!r)
    {
    system("clear");
    printf("\n\n\t\t======= ATM =======\n");
    printf("\n\t\t-->> Feel free to login / register :\n");
    printf("\n\t\t[1]- login\n");
    printf("\n\t\t[2]- register\n");
    printf("\n\t\t[3]- exit\n");
    printf("\n\t\t");
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
            if(registerAcc(u->name, u->password))
            {
                r = errorMessage();
                break;
            }
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

int checkUserExists(char *name)
{
    FILE *fp = fopen("./data/users.txt", "r");
    if (fp == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }

    int count = 0;
    int c = getc(fp);
    if (c != EOF) { // check if file is empty
        while (c != EOF) {
            if (c == '\n') { // Increment count if this character is newline 
                count++;
            }
            c = getc(fp);
        }

        rewind(fp); // move the file pointer back to the beginning of the file

        char line[103];
        while (fgets(line, sizeof(line), fp) != NULL) {
            int id;
            char username[50];
            sscanf(line, "%d %s", &id, username);
            if (strcmp(username, name) == 0) {
                fclose(fp);
                return 1;
            }
        }
    }

    fclose(fp);
    return 0;
}



int registerAcc(char name[50], char password[50])
{
    FILE *f = fopen("./data/users.txt", "a+");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    char name[50], password[50];

    printf("\t\tEnter name: ");
    scanf("%s", name);
    printf("\t\tEnter password: ");
    scanf("%s", password);

    if (checkUserExists(name)) {
        fclose(f);
        return 1;
    }

    // Read last id from file
    // int id, count;
    // if (fscanf(f, "%d", &count) != 1) {
    //     id = 0;
    // } else {
    //     id = count;
    // }

    int count = 0;
    for (int c = getc(f); c != EOF; c = getc(f))
    {
      if (c == '\n')
      { // Increment count if this character is newline 
        count++;
      }
    }

    // Write user's details to file
    fprintf(f, "%d ", count);
    fprintf(f, "%s %s\n", name, password);
    fclose(f);
    return 0;
}


int errorMessage(){
    int selection = 0;
    system("clear");
    printf("\n\n\t\tError: user with the same name already exists!\n\n");
    printf("\t\t[1]- return\n");
    printf("\t\t[2]- exit\n");
    while(!selection) {
        printf("\n\t\t");
        scanf("%d", &selection);
        switch(selection){
            case 1: return 0;
            case 2: exit(1);
            default:
                printf("\t\tIncorrect input. \n");
                selection = 0;
                break;
        }
    }


}

int main()
{
    struct User u;
    
    initMenu(&u);
    mainMenu(u);
    return 0;
}
