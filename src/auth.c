#include <termios.h>
#include "header.h"

char *USERS = "./data/users.txt";

void loginMenu(char a[50], char pass[50])
{
    // Declare two termios structures for storing terminal flags
    struct termios oflags, nflags;

    // Clear the screen
    system("clear");

    // Print the login menu
    printf("\n\n\n\t\t\t\t  Online Bank Management System\n\t\t\t\t\t User Login:");
    scanf("%s", a);

    // Get the current terminal flags
    tcgetattr(fileno(stdin), &oflags);

    // Make a copy of the flags
    nflags = oflags;

    // Modify the flags to disable echo
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;

    // Set the new flags for the terminal
    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0)
    {
        // If there was an error setting the flags, print an error message
        perror("tcsetattr");

        // Exit the program with an error code
        return exit(1);
    }

    // Prompt the user for the password
    printf("\n\n\n\n\n\t\t\t\tEnter the password to login:");
    scanf("%s", pass);

    // Restore the original terminal flags
    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
    {
        // If there was an error setting the flags, print an error message
        perror("tcsetattr");

        // Exit the program with an error code
        return exit(1);
    }
};


const char *getPassword(struct User u)
{
FILE *fp; // declare a file pointer
struct User userChecker; // declare a User object

   if ((fp = fopen("./data/users.txt", "r")) == NULL) // try to open "./data/users.txt" for reading
{
    printf("Error! opening file");
    exit(1); // exit with an error if the file cannot be opened
}

while (fscanf(fp, "%s %s", userChecker.name, userChecker.password) != EOF) // read the file line by line
{
    if (strcmp(userChecker.name, u.name) == 0) // compare the user name of the current line to the user name passed to the function
    {
        fclose(fp); // close the file
        char *buff = userChecker.password; // copy the password to a new buffer
        return buff; // return the buffer
    }
}

fclose(fp); // close the file
return "no user found!"; // return "no user found!" if the end of the file is reached without finding a match
}




void registerMenu(char a[50], char pass[50]) {
    struct termios oflags, nflags;

    system("clear"); // clear the screen

    printf("\n\n\n\t\t\t\t  Online Bank Management System\n\t\t\t\t\t Username to Register:");
    scanf("%s", a); // read the user name from the input

    // disabling echo
    tcgetattr(fileno(stdin), &oflags); // get the current terminal attributes
    nflags = oflags; // copy the attributes to another variable
    nflags.c_lflag &= ~ECHO; // disable echo
    nflags.c_lflag |= ECHONL; // enable newline echo

    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0) { // try to set the new terminal attributes
        perror("tcsetattr"); // print an error message if it fails
        return exit(1); // exit with an error code
    }

    printf("\n\n\n\n\n\t\t\t\tEnter the password for registration:");
    scanf("%s", pass); // read the password from the input

    // restore terminal
    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0) { // try to restore the original terminal attributes
        perror("tcsetattr"); // print an error message if it fails
        return exit(1); // exit with an error code
    }

    FILE *f, *f2;
    if ((f = fopen("./data/users.txt", "a+")) == NULL) // try to open "./data/users.txt" for appending and reading
    {
        printf("Error! opening file"); // print an error message if the file cannot be opened
        return exit(1); // exit with an error code
    }

    char userName[100];

    if ((f2 = fopen("./data/users.txt", "r+")) == NULL) { // try to open "./data/users.txt" for reading and updating
    printf("ERROR! opening file\n"); // print an error message if the file cannot be opened
    return exit(1); // exit with an error code
    }
    while(fscanf(f2, "%s", userName) != EOF) { // read the file line by line
        if (strcmp(userName, a) == 0) { // compare the user name read from the file to the user name passed to the function
            printf("ERROR! Username already exists\n"); // print an error message if the user name already exists
            return exit(1); // exit with an error code
        }
    }

    fprintf(f, "%s %s\n", a, pass); // write the user name and password to the file
    fclose(f); // close the file
}
