#include <stdlib.h>
#include <stdio.h>
#include <client.h>
#include <server.h>
#include <string.h>
#include <common.h>

void GetUserName(char[USER_NAME_LENGTH]);

int main()
{
        int input;
        char userName[USER_NAME_LENGTH];
        int countOfErrors = 0;
        int isProgramRuuning = FALSE;
        printf("Wellcome to crypto - chat with encrypting!\n");
        GetUserName(userName);
        do
        {
            printf("Enter 1 to run client or 2 to run server:\n");
            if (!scanf("%d", &input))
            {
                perror("input");
            }
            switch (input)
            {
                case 1:
                    RunClient();
                    isProgramRuuning = TRUE;
                    break;
                case 2:
                    RunServer();
                    isProgramRuuning = TRUE;
                    break;
                default:
                    printf("Incorrect input value. Try again.\n");
                    countOfErrors++;
                    break;
            }
        }
        while ((!isProgramRuuning) & (countOfErrors <3));
        if ((countOfErrors >= 3) && (!isProgramRuuning))
        {
            printf("The limits of attempts was exceeded. App is closing...\n");
        }
        return 0;
}

void GetUserName(char userName[USER_NAME_LENGTH])
{
    int isReadingSuccesfull;
    do
    {
        isReadingSuccesfull = TRUE;
        printf("Enter your username. Length must be less %d.\nYou can use any characters except \":\" \n", USER_NAME_LENGTH);
        if (!scanf("%s",userName))
        {
            printf("Incorrect username.\n");
            isReadingSuccesfull = FALSE;
        }
        if (strchr(userName, ':'))
        {
            printf("Incorrect username.\n");
            isReadingSuccesfull = FALSE;
        }
    }
    while (!isReadingSuccesfull);
}

