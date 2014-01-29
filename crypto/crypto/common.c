#include <common.h>
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <string.h>

static void SetErrors(char* paramsName, int *countOfErrors, int *isReadingSuccesfull);
static void GetPasswordByCharacter(char password[PARAMETRS_LENGTH], char* message);

void GetPassword(char password[PARAMETRS_LENGTH], int passwordOwner)
{
    char message[128];
    char passwordVerification[PARAMETRS_LENGTH];
    sprintf(message, "Please, enter your password. Length must be less than %d and greater than 5.\nYou can use any characters except \":\" ", PARAMETRS_LENGTH);
    GetPasswordByCharacter(password, message);
    if (passwordOwner == PASS_SERVER)
    {
        sprintf(message, "Retype the password:");
        GetPasswordByCharacter(passwordVerification, message);
        if (strcmp(password, passwordVerification) != 0)
        {
            printf("Entered password do not match. App is closing...\n");
            exit(0);
        }
    }
}

static void GetPasswordByCharacter(char password[PARAMETRS_LENGTH], char* message)
{
    int isReadingSuccesfull, countOfErrors,i;
    struct termios oldt, newt;
    char c;
    i = 0;
    countOfErrors = 0;
    do
    {
        isReadingSuccesfull = TRUE;

        tcgetattr ( 0, &oldt );
        newt = oldt;

        newt.c_lflag &= ~( ICANON | ECHO );
        tcsetattr ( 0, TCSANOW, &newt );
        printf(message);
        puts("");
        while (1)
        {
            if (read(0, &c, sizeof(c)) > 0)
            {
                if ('\n' == c)
                {

                    if (i < 5)
                    {
                        i = 0;
                        SetErrors("password", &countOfErrors, &isReadingSuccesfull);
                    }
                    if (i > PARAMETRS_LENGTH)
                    {
                        i = 0;
                        SetErrors("password", &countOfErrors, &isReadingSuccesfull);
                    }
                    password[i]='\0';
                    break;
                }
                if (c == ':')
                {
                    i = 0;
                    SetErrors("password", &countOfErrors, &isReadingSuccesfull);
                    break;
                }
                password[i++]=c;
            }
        }
        tcsetattr ( 0, TCSANOW, &oldt );
    }
    while (isReadingSuccesfull == FALSE && countOfErrors <ERRORS_LIMIT);
    if (isReadingSuccesfull == FALSE)
    {
        printf("The limits of attempts was exceeded. App is closing...\n");
        exit(1);
    }
}

void GetUserName(char userName[PARAMETRS_LENGTH])
{
    int isReadingSuccesfull, countOfErrors;
    countOfErrors = 0;
    do
    {
        isReadingSuccesfull = TRUE;
        printf("Please, enter your username. Length must be less than %d.\nYou can use any characters except \":\" \n", PARAMETRS_LENGTH);
        if (scanf("%s",userName) == 0)
        {
            SetErrors("username", &countOfErrors, &isReadingSuccesfull);
        }
        if (strchr(userName, ':'))
        {
            SetErrors("username", &countOfErrors, &isReadingSuccesfull);
        }
        if (strlen(userName) >= 16)
        {
            SetErrors("username", &countOfErrors, &isReadingSuccesfull);
        }
    }
    while (isReadingSuccesfull == FALSE && countOfErrors <ERRORS_LIMIT);
    if (isReadingSuccesfull == FALSE)
    {
        printf("The limits of attempts was exceeded. App is closing...\n");
        exit(0);
    }
}

static void SetErrors(char* paramsName, int *countOfErrors, int *isReadingSuccesfull)
{
    printf("Incorrect %s. Try again.\n", paramsName);
    *isReadingSuccesfull = FALSE;
    (*countOfErrors)++;
}

void FixRecievingError(int recievedBytesCount, int *socketFD, char* errorMessage)
{
    if (recievedBytesCount == 0)
    {
        printf("%s",errorMessage);
        //printf("Connetion error occured.\n");
    }
    else
    {
        perror("recv");
    }
    close(*socketFD);
}
