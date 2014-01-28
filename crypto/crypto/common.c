#include <common.h>
#include <stdlib.h>
#include <stdio.h>

static void SetErrors(char* paramsName, int *countOfErrors, int *isReadingSuccesfull);

void GetPassword(char password[PARAMETRS_LENGTH])
{
    int isReadingSuccesfull, countOfErrors;
    countOfErrors = 0;
    do
    {
        isReadingSuccesfull = TRUE;
        printf("Please, enter your password. Length must be less %d.\nYou can use any characters except \":\" \n", PARAMETRS_LENGTH);
        if (scanf("%s",password) == 0)
        {
            SetErrors("password", &countOfErrors, &isReadingSuccesfull);
        }
        if (strchr(password, ':'))
        {
            SetErrors("password", &countOfErrors, &isReadingSuccesfull);
        }
        if (strlen(password) >= 16)
        {
            SetErrors("password", &countOfErrors, &isReadingSuccesfull);
        }
    }
    while (isReadingSuccesfull == FALSE && countOfErrors <ERRORS_LIMIT);
    if (isReadingSuccesfull == FALSE)
    {
        printf("The limits of attempts was exceeded. App is closing...\n");
        exit(0);
    }
}

void GetUserName(char userName[PARAMETRS_LENGTH])
{
    int isReadingSuccesfull, countOfErrors;
    countOfErrors = 0;
    do
    {
        isReadingSuccesfull = TRUE;
        printf("Please, enter your username. Length must be less %d.\nYou can use any characters except \":\" \n", PARAMETRS_LENGTH);
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
