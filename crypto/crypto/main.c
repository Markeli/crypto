#include <stdlib.h>
#include <stdio.h>
#include <client.h>
#include <server.h>

#define TRUE 1
#define FALSE 0

int main()
{
        char input;
        int countOfErrors = 0;
        int isProgramRuuning = FALSE;
        printf("Wellcome to crypto - chat with encrypting!\n");
        do
        {
            printf("Enter 1 to run client or 2 to run server:\n");
            input = getchar();
            switch (input)
            {
                case '1':
                    RunClient();
                    isProgramRuuning = TRUE;
                    break;
                case '2':
                    RunServer();
                    isProgramRuuning = TRUE;
                    break;
                default:
                    printf("Incorrect input value. Try again.\n");
                    countOfErrors++;
                    break;
            }
        }
        while ((!isProgramRuuning) | (countOfErrors <3));
        if ((countOfErrors >= 3) & (!isProgramRuuning))
        {
            printf("The limits of attempts was exceeded. App is closing...");
        }
        return 0;
}

