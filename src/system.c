#include "header.h" // include the header file containing the function declarations
#include <stdio.h> // include the standard I/O library

#include <stdlib.h> // include the standard library
#include <unistd.h> // include the UNIX standard library

#include <string.h> // include the string library
#include <errno.h> // include the error library
#include <unistd.h> // include the UNIX standard library
#include <sys/wait.h> // include the wait library
#include <sys/types.h> // include the system types library
#include <sys/stat.h> // include the system stat library
#include <fcntl.h> // include the file control library
#include <sys/socket.h> // include the socket library
#include <netinet/in.h> // include the internet library
#include <arpa/inet.h> // include the internet library

#define PORT 8080

const char *RECORDS = "./data/records.txt";

// This function reads account information from a file and stores it in a struct
// It takes a file pointer, a string for the account owner's name, and a pointer to a struct for the record
// It returns 1 if the read was successful and 0 if the end of the file was reached

int getAccountFromFile(FILE *ptr, char name[50], struct Record *r)
{
    // try to read the account information from the file and store it in the provided struct
// return 1 if the read was successful, 0 if the end of the file was reached
    return fscanf(ptr, "%s %d %d/%d/%d %s %lld %lf %s",
                  name,
                  &r->accountNbr,
                  &r->deposit.month,
                  &r->deposit.day,
                  &r->deposit.year,
                  r->country,
                  &r->phone,
                  &r->amount,
                  r->accountType) != EOF;
}

// This function reads user information from a file and stores it in a struct
// It takes a file pointer, a string for the user's name, and a pointer to a struct for the user's information
// It returns 1 if the read was successful and 0 if the end of the file was reached
int getUsersFromFile(FILE *ptr, char name[50], struct User *u) {
// try to read the user information from the file and store it in the provided struct
// return 1 if the read was successful, 0 if the end of the file was reached
    return fscanf(ptr, "%s %s,", name, u->password) != EOF;
}

// This function writes an account's information to a file
// It takes a file pointer, a string for the account owner's name, and a struct containing the account information
void saveAccountToFile(FILE *ptr, char name[50], struct Record r)
{
// write the account information to the file using the provided file pointer and struct
    fprintf(ptr, "%s %d %d/%d/%d %s %lld %.2lf %s\n\n",
        name,
        r.accountNbr,
        r.deposit.month,
        r.deposit.day,
        r.deposit.year,
        r.country,
        r.phone,
        r.amount,
        r.accountType);
}


// This function allows the user to either return to the main menu or exit the program
// It takes a flag to indicate whether the previous operation was successful, a pointer to a function to return to,
// and a struct containing the user's information
void stayOrReturn(int notGood, void f(struct User u, int sysClear), struct User u)
{
    int option;
    if (notGood == 0) // if the previous operation was not successful
    {
        system("clear");
        printf("\n✖ Record not found!!\n");
    invalid:
        printf("\nEnter 0 to try again, 1 to return to main menu and 2 to exit:"); // prompt the user for their next action
        scanf("%d", &option); // read the user's selected option
        if (option == 0) // if the user chose to try the operation again
            f(u, 1); // call the provided function again
        else if (option == 1) // if the user chose to return to the main menu
            mainMenu(u, 1);  // call the main menu function
        else if (option == 2) // if the user chose to exit the program
            exit(0); // exit the program with a success code
        else // if the user entered an invalid option
        {
            printf("Insert a valid operation!\n"); // print an error message
            goto invalid; // go back to the prompt
        }
    }
    else
    {
        printf("\nEnter 1 to go to the main menu and 0 to exit:");
        scanf("%d", &option);
        }
        if (option == 1)
        {
            system("clear");
            mainMenu(u, 1);
        }
        else
        {
            system("clear");
            exit(1);
        }
}


/*
The success() function is a utility function that is called after a successful operation has been performed. 
It takes a struct User as an argument and displays a success message to the user.
The invalid label is used as a target for the goto statement later in the function. 
This allows the code to jump to the invalid label if the user enters an invalid option.
The printf() and scanf() statements are used to display a message to the user and read in their response, respectively. 
The system("clear") call is used to clear the terminal screen.
The if and else if statements are used to check the value of option and determine what action to take next. 
If option is 1, the mainMenu() function is called with u and 1 as arguments. If option is 0, the program is terminated using the exit() function. If option is neither 1 nor 0, an error message is displayed and the code jumps back to the invalid label using the goto statement.
*/
void success(struct User u)
{
    int option;
    printf("\n✔ Success!\n\n");
invalid:
    printf("Enter 1 to go to the main menu and 0 to exit!\n");
    scanf("%d", &option);
    system("clear");
    if (option == 1)
    {
        mainMenu(u, 1);
    }
    else if (option == 0)
    {
        exit(1);
    }
    else
    {
        printf("Insert a valid operation!\n");
        goto invalid;
    }
}

void createNewAcc(struct User u)
{
    struct Record r;
    struct Record cr;
    char userName[50];
    int Num;

noAccount:
    system("clear");
    printf("\t\t\t===== New record =====\n");

    // need to add current date
    printf("\nEnter today's date(mm/dd/yyyy):");
    if (scanf("%d/%d/%d", &r.deposit.month, &r.deposit.day, &r.deposit.year) != 3) {
        printf("enter the date correctly\n");
        sleep(3);
        goto noAccount;
    }
    printf("\nEnter the account number:");
    scanf("%d", &r.accountNbr);

    FILE *pf = fopen(RECORDS, "a+");
    while (getAccountFromFile(pf, userName, &cr))
    {
        if (/*strcmp(userName, u.name) == 0 && */ cr.accountNbr == r.accountNbr)
        {
            printf("✖ This Account number already exists \n\n");
            sleep(3);
            goto noAccount;
        }
    }
    printf("\nEnter the country:");
    scanf("%s", r.country);
    printf("\nEnter the phone number:");
    scanf("%lld", &r.phone);
    printf("\nEnter amount to deposit: $");
    scanf("%lf", &r.amount);
    printf("\nChoose the type of account:\n\t1 -> saving\n\t2 -> current\n\t3 -> fixed01(for 1 year)\n\t4 -> fixed02(for 2 years)\n\t5 -> fixed03(for 3 years)\n\n\tEnter your choice:");
    scanf("%d", &Num);
    if (Num == 1) {
        strcpy(r.accountType, "saving");
    } 
    else if (Num == 2) {
        strcpy(r.accountType, "current");
    }
    else if (Num == 3) {
        strcpy(r.accountType, "fixed01");
    }
    else if (Num == 4) {
        strcpy(r.accountType, "fixed02");
    }
    else if (Num == 5) {
        strcpy(r.accountType, "fixed03");
    }
    else {
        printf("Invalid operation");
        goto noAccount;
    }
    saveAccountToFile(pf, u.name, r);

    fclose(pf);
    success(u);
}

void updateAcc(struct User u) {
    int AccNum, Num;
    signed long long int Phone;
    char Country[100];

    FILE * original;
    struct Record r;
    char userName[50];

    if ((original = fopen(RECORDS, "r+")) == NULL)  { // try to open the file for reading and writing
        printf("Error! opening file"); // if the file cannot be opened, print an error message
        exit(1); // and exit the program
    }

    printf("\n\nThe account number you want to change: ");
    scanf("%d", &AccNum);
    int ok = 0; // initialize a flag variable to 0
    while(getAccountFromFile(original, userName, &r)) { // read in a record from the file
        if (strcmp(userName, u.name) == 0 && AccNum == r.accountNbr) {
            // if the username and account number match the user's input
            ok = 1; // set the flag variable to 1
        }      
    }
    if (ok == 0) {
        printf("\nInvalid Account Number");
        stayOrReturn(1, mainMenu, u); 
        // display a message and allow the user to go back to the main menu or exit the program
    }
    
    printf("\nWhich information do you want to update?\n 1 -> phone number\n 2-> country\n");
    scanf("%d", &Num);

    system("clear");

    if (Num == 1) {
        printf("Enter the new phone number: ");
        scanf("%lld", &Phone);

    //%lld is a format specifier in the printf() and scanf() functions in C programming. 
    // It stands for "long long decimal" and is used to specify that the corresponding argument is a long long integer. 
    // It is similar to the %d format specifier, but can be used to print and scan values that are too large to be represented by a regular int.
    } 
    else if (Num == 2) {
        printf("Enter the new country: ");
        scanf("%s", Country);
    }
    else {
        printf("Invalid operation!\n");
        stayOrReturn(1, mainMenu, u);
    }
    FILE * new;
    if ((new = fopen( "./data/NewRecords.txt", "w")) == NULL) {
        printf("Error! opening file");
        exit(1);
    }

    struct Record r2;
    userName[0] = '\0';
    
    if ((original = fopen(RECORDS, "r+")) == NULL)  {
        printf("Error! opening file");
        exit(1);
    }

    while (getAccountFromFile(original, userName, &r2)) {
        if (strcmp(userName, u.name) == 0 && AccNum == r2.accountNbr) {
            if (Num == 1) {
                r2.phone = Phone;
            }
            if (Num == 2) {
                strcpy(r2.country, Country);
            }
        }
        saveAccountToFile(new, userName, r2);
    }

    remove("./data/records.txt");
    rename("./data/NewRecords.txt", "./data/records.txt");

    fclose(original); 
    fclose(new);
    success(u);
}

///////////////////
void CheckAcc(struct User u) {
    int AccNum, date, d, m, y;
    char userName[100], AccType[100];
    struct Record r;
    float InterestRate = 1.0;
    int ok = 0;

Again:
    printf("Enter the account number: ");
    scanf("%d", &AccNum);

    system("clear");

    FILE * f = fopen(RECORDS, "r");

    if ((f = fopen(RECORDS, "r+")) == NULL)  {
        printf("Error! opening file");
        exit(1);
    }
    while (getAccountFromFile(f, userName, &r))
    {
        if (strcmp(userName, u.name) == 0 && AccNum == r.accountNbr)
        {
            ok++;
            printf("_____________________\n");
            printf("\nAccount number:%d\nDeposit Date:%d/%d/%d \ncountry:%s \nPhone number:%lld \nAmount deposited: $%.2f \nType Of Account:%s\n",
                   r.accountNbr,
                   r.deposit.day,
                   r.deposit.month,
                   r.deposit.year,
                   r.country,
                   r.phone,
                   r.amount,
                   r.accountType);
            if (strcmp(r.accountType, "saving") == 0) {
                InterestRate = 0.07;
                strcpy(AccType, "saving");
                date = r.deposit.day;
            }
            if (strcmp(r.accountType, "current") == 0) {
                InterestRate = 0.0;
                strcpy(AccType, "current");
                d = r.deposit.day;
                m = r.deposit.month;
                y = r.deposit.year;
            }
            if (strcmp(r.accountType, "fixed01") == 0) {
                InterestRate = 0.04;
                strcpy(AccType, "fixed01");
                d = r.deposit.day;
                m = r.deposit.month;
                y = r.deposit.year;
            }
            if (strcmp(r.accountType, "fixed02") == 0) {
                InterestRate = 0.05;
                strcpy(AccType, "fixed02");
                d = r.deposit.day;
                m = r.deposit.month;
                y = r.deposit.year;
            }
            if (strcmp(r.accountType, "fixed03") == 0) {
                InterestRate = 0.08;
                strcpy(AccType, "fixed03");
                d = r.deposit.day;
                m = r.deposit.month;
                y = r.deposit.year;
            }

        }
    }
    if (ok == 0) {
        printf("Invalid operation\n\n");
        stayOrReturn(1, mainMenu, u);
        goto Again;
    }

    if (InterestRate == 0.0) {
        printf("\n\nYou will not get interests because the account is of type current");
    } else {
        if (strcmp(AccType, "saving") == 0) {
            printf("\n\nYou will get $%f as interest on day %d of every mounth\n", (r.amount * InterestRate) / 12, date);
        }
        if (strcmp(AccType, "fixed01") == 0) {
            printf("\n\nYou will get $%f as interest on %d/%d/%d", (r.amount * InterestRate), d, m, y+1);
        }
        if (strcmp(AccType, "fixed02") == 0) {
            printf("\n\nYou will get $%f as interest on %d/%d/%d", (r.amount * InterestRate) * 2, d, m, y+2);
        }
        if (strcmp(AccType, "fixed03") == 0) {
            printf("\n\nYou will get $%f as interest on %d/%d/%d", (r.amount * InterestRate) * 3, d, m, y+3);
        }

    }
    success(u);
}


////////////////
void checkAllAccounts(struct User u)
{
    char userName[100];
    struct Record r;

    FILE *pf = fopen(RECORDS, "r");

    if (pf == NULL) {
        printf("Error! opening file");
        exit(1);
    }

    system("clear");
    printf("\t\t====== All accounts from user, %s =====\n\n", u.name);
    while (getAccountFromFile(pf, userName, &r))
    {
        if (strcmp(userName, u.name) == 0)
        {
            printf("_____________________\n");
            printf("\nAccount number:%d\nDeposit Date:%d/%d/%d \ncountry:%s \nPhone number:%lld \nAmount deposited: $%.2f \nType Of Account:%s\n",
                   r.accountNbr,
                   r.deposit.day,
                   r.deposit.month,
                   r.deposit.year,
                   r.country,
                   r.phone,
                   r.amount,
                   r.accountType);
        }
    }
    fclose(pf);
    success(u);
}

void MakeTransaction(struct User u) {
    int AccNum;
    printf("\nEnter the account number of the customer: ");
    scanf("%d", &AccNum);

    FILE * f;
    if ((f = fopen(RECORDS, "r+")) == NULL)  {
        printf("Error! opening file");
        exit(1);
    }

    struct Record r;
    char userName[100];
    int ok = 0;
    while(getAccountFromFile(f, userName, &r)) {
        if ((strcmp(userName, u.name) == 0) && AccNum == r.accountNbr) {
            ok = 1;
        }
    }
    fclose(f);

    if (ok == 0) {
        printf("Invalid operation\n\n");
        stayOrReturn(1, mainMenu, u);
    }

    int Num;
    printf("\n\nDo you want to:\n\t1-> Withdraw\n\t2-> Deposit\n\nEnter your choice: ");
    scanf("%d", &Num);

    if ((Num != 1) && (Num != 2)) {
        printf("Invalid operation\n\n");
        stayOrReturn(1, mainMenu, u);
    }

    double Sum;
    if ((Num == 1)){
    printf("Enter the amount you want to withdraw: $");
    scanf("%lf", &Sum);
    }
    if ( (Num == 2)){
    printf("Enter the amount you want to deposit: $");
    scanf("%lf", &Sum);
    }

    FILE * f2;
    struct Record r2;
    userName[0] = '\0';

    if ((f2 = fopen(RECORDS, "r+")) == NULL)  {
        printf("Error! opening file");
        exit(1);
    }
    
    FILE * new;
    if ((new = fopen("./data/NewRecords.txt", "w+")) == NULL)  {
        printf("Error! opening file");
        exit(1);
    }

    ok = 0;
    while(getAccountFromFile(f2, userName, &r2)) {
        if ((strcmp(userName, u.name) == 0) && AccNum == r2.accountNbr) {
            if (r2.accountType[0] == 'f') {
                    system("clear");
                    printf("✖ You cannot deposit or withdraw cash in fixed accounts\n");
                    stayOrReturn(1, mainMenu, u);
            } 
            else if (Num == 1) {
                if (Sum > r2.amount) {
                    system("clear");
                    printf("✖ Insufficient balance for the operation\n");
                    stayOrReturn(1, mainMenu, u);
                } else {
                    r2.amount -= Sum;
                    ok++;
                }
            }
            else if (Num == 2) {
                r2.amount += Sum;
                ok++;
            }
        }
        saveAccountToFile(new, userName, r2);
    }
    fclose(f2);
    if (ok > 0) {
        remove("./data/records.txt");
        rename("./data/NewRecords.txt", "./data/records.txt");
    }

    
    fclose(new);
    printf("Transaction successful\n");
    success(u);
}


void RemoveAcc(struct User u) {
    int AccNum;

    printf("\n\nEnter the account number you want to delete: ");
    scanf("%d", &AccNum);

    FILE * f;
    if ((f = fopen(RECORDS, "r+")) == NULL)  {
        printf("Error! opening file");
        exit(1);
    }

    struct Record r;
    char userName[100];
    int ok = 0;

    FILE * new;
    if ((new = fopen("./data/NewRecords.txt", "w+")) == NULL)  {
        printf("Error! opening file");
        exit(1);
    }

    while(getAccountFromFile(f, userName, &r)) {
        if ((strcmp(userName, u.name) == 0) && AccNum == r.accountNbr) {
            ok = 1;
            system("clear");
            printf("_____________________\n");
            printf("\nAccount number:%d\nDeposit Date:%d/%d/%d \ncountry:%s \nPhone number:%lld \nAmount deposited: $%.2f \nType Of Account:%s\n",
                   r.accountNbr,
                   r.deposit.day,
                   r.deposit.month,
                   r.deposit.year,
                   r.country,
                   r.phone,
                   r.amount,
                   r.accountType);
        } else {
             saveAccountToFile(new, userName, r);
        }
    }
    if (ok == 0) {
        printf("Invalid operation. Account doesn't exist.\n\n");
        stayOrReturn(1, mainMenu, u);
    }

    remove("./data/records.txt");
    rename("./data/NewRecords.txt", "./data/records.txt");

    fclose(f); 
    fclose(new);

    success(u);
}

void TransferAcc(struct User u) {
    int AccNum;

    printf("\n\nEnter the account number you want to transfere ownership: ");
    scanf("%d", &AccNum);
    

    struct Record r;
    char userName[100];
    int ok = 0, ok2 = 0;
    char nickName[100];
    struct User u2;

    printf("\nWhich user you want transfer ownership to (user name): ");
    scanf("%s", nickName);

    FILE * new;
    if ((new = fopen("./data/NewRecords.txt", "w+")) == NULL)  {
        printf("Error! opening file");
        exit(1);
    }

    FILE * f, * f2;
    if ((f = fopen(RECORDS, "r+")) == NULL)  {
        printf("Error! opening file");
        exit(1);
    }
    if ((f2 = fopen("./data/users.txt", "r+")) == NULL) {
        printf("ERROR! opening file");
        exit(1);
    }
    while(getUsersFromFile(f2, userName, &u2)) {
        if (strcmp(userName, nickName) == 0) {
            ok2++;
        }
    }
    userName[0] = '\0';
    while(getAccountFromFile(f, userName, &r)) {
        if ((strcmp(userName, u.name) == 0) && AccNum == r.accountNbr) {
            ok = 1;
            printf("\nAccount number:%d\nDeposit Date:%d/%d/%d \ncountry:%s \nPhone number:%lld \nAmount deposited: $%.2f \nType Of Account:%s\n",
                   r.accountNbr,
                   r.deposit.day,
                   r.deposit.month,
                   r.deposit.year,
                   r.country,
                   r.phone,
                   r.amount,
                   r.accountType);
            strcpy(userName, nickName);
        }
        saveAccountToFile(new, userName, r);

    }
    if (ok == 0 || ok2 == 0) {
        printf("\n\nInvalid input\n\n");
        stayOrReturn(1, mainMenu, u);
    } else {
        remove("./data/records.txt");
        rename("./data/NewRecords.txt", "./data/records.txt");       
    }
    fclose(f); 
    fclose(new);

    int socket_desc;
	struct sockaddr_in server;
	char *message , server_reply[50];
	
	//Create socket
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1)
	{
		printf("Could not create socket");
	}
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);    

	//Connect to remote server
	if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0)
	{
		puts("connect error");
	}
	
	puts("Connected\n");
	
	//Send some data
	message = u.name;
	if( send(socket_desc , message , strlen(message) , 0) < 0)
	{
		puts("Send failed");
	}
	puts("Data Send\n");
	
    success(u);
}