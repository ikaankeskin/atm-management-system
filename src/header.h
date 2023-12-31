#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// This struct represents a date, with separate fields for the month, day, and year
struct Date
{
int month, day, year;
};

// This struct represents a record of an account, with fields for the account owner's name, country, phone number,
// account type, account number, account balance, deposit date, and withdrawal date
struct Record
{
char name[100];
char country[100];
signed long long int phone;
char accountType[10];
int accountNbr;
double amount;
struct Date deposit;
struct Date withdraw;
};

struct User
{
char name[50];
char password[50];
};

// These are functions related to authenticating users
void loginMenu(char a[50], char pass[50]);
void registerMenu(char a[50], char pass[50]);
const char *getPassword(struct User u);

// These are functions related to managing accounts
void createNewAcc(struct User u);
void updateAcc(struct User u);
void CheckAcc(struct User u);
void MakeTransaction(struct User u);
void RemoveAcc(struct User u);
void TransferAcc(struct User u);
void mainMenu(struct User u, int sysClear);
void checkAllAccounts(struct User u);