#include <common.h>
#include <stdlib.h>
#include <stdio.h>

static void SetErrors(char* paramsName, int* countOfErrors, int* isReadingSuccesfull);

void GetParametr(char parametr[PARAMETRS_LENGTH], char* paramsName)
{
    int isReadingSuccesfull, countOfErrors;
    countOfErrors = 0;
    do
    {
        isReadingSuccesfull = TRUE;
        printf("Please, enter your %s. Length must be less %d.\nYou can use any characters except \":\" \n",paramsName, PARAMETRS_LENGTH);
        if (!scanf("%s",parametr))
        {
            SetErrors(paramsName, &countOfErrors, &isReadingSuccesfull);
        }
        if (strchr(parametr, ':'))
        {
            SetErrors(paramsName, &countOfErrors, &isReadingSuccesfull);
        }
        if (strlen(parametr) >= 16)
        {
            SetErrors(paramsName, &countOfErrors, &isReadingSuccesfull);
        }
    }
    while (isReadingSuccesfull == FALSE && countOfErrors <ERRORS_LIMIT);
    if (isReadingSuccesfull == FALSE)
    {
        printf("The limits of attempts was exceeded. App is closing...\n");
        exit(0);
    }
}

static void SetErrors(char* paramsName, int* countOfErrors, int* isReadingSuccesfull)
{
    printf("Incorrect %s. Try again.\n", paramsName);
    *isReadingSuccesfull = FALSE;
    (*countOfErrors)++;
}
