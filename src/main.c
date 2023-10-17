#include "header.h" // include the header file containing the function declarations

#include <stdio.h> // include the standard I/O library

#include <string.h> // include the string library
#include <stdlib.h> // include the standard library
#include <unistd.h> // include the UNIX standard library
#include <sys/wait.h> // include the wait library
#include <errno.h> // include the error library
#include <fcntl.h> // include the file control library

#include <sys/types.h> // include the system types library
#include <sys/stat.h> // include the system stat library
#include <sys/socket.h> // include the socket library
#include <arpa/inet.h> // include the internet library
#include <netinet/in.h> // include the internet library

#include <pthread.h> // include the POSIX thread library

void mainMenu(struct User u, int sysClear)
{
    int option;

    if (sysClear == 1) {
     system("clear");
    }
    char nick[100];
    printf("\n\n\t\t======= ATM =======\n\n");
    printf("\n\t\t-->> Feel free to choose one of the options below <<--\n");
    printf("\n\t\t[1]- Create a new account\n");
    printf("\n\t\t[2]- Update information of account\n");
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
        updateAcc(u);
        break;
    case 3:
        CheckAcc(u);
        break;
    case 4:
        checkAllAccounts(u);
    case 5:
        MakeTransaction(u);
        break;
    case 6:
        RemoveAcc(u);
        break;
    case 7:
        TransferAcc(u);
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
    while (!r) // keep looping until the user has selected an option
    {
        scanf("%d", &option); // read the user's selected option
        switch (option)
        {
        case 1: // if the user selected 1 (login)
            loginMenu(u->name, u->password); // call the login menu function
            if (strcmp(u->password, getPassword(*u)) == 0) // compare the entered password to the correct password
            {
            printf("\n\nPassword Match!"); // print a success message if the passwords match
            }
            else
            {
            printf("\nWrong password!! or User Name\n"); // print an error message if the passwords don't match
            exit(1); // exit with an error code
            }
            r = 1; // set r to 1 to exit the loop
            break;
        case 2: // if the user selected 2 (register)
            registerMenu(u->name, u->password); // call the registration menu function
            r = 1; // set r to 1 to exit the loop
            break;
        case 3: // if the user selected 3 (exit)
            exit(1); // exit the program with an error code
            break;
        default: // if the user entered an invalid option
            printf("Insert a valid operation!\n"); // print an error message
            exit(1); // exit the program with an error code
        }
    }
};

int main()
{

    struct User u;
    initMenu(&u);
    mainMenu(u, 1);
    return 0;
}
