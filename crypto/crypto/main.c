#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <common.h>
#include <client.h>
#include <server.h>

void GetUserName(char[PARAMETRS_LENGTH]);

int main()
{
        int mode;
        char userName[PARAMETRS_LENGTH];
        int countOfErrors = 0;
        int isProgramRuuning = FALSE;
        printf("Wellcome to crypto - chat with encrypting!\n\v");
        GetParametr(userName, "username\0");
        do
        {
            printf("Enter 1 to run client or 2 to run server:\n");
            scanf("%d", &mode);
            {
                switch (mode)
                {
                    case 1:
                        RunClient(userName);
                        isProgramRuuning = TRUE;
                        break;
                    case 2:
                        RunServer(userName);
                        isProgramRuuning = TRUE;
                        break;
                    default:
                        printf("Incorrect input value. Try again.\n");
                        countOfErrors++;
                        break;
                }
            }
        }
        while (isProgramRuuning == FALSE && countOfErrors < ERRORS_LIMIT);
        if (isProgramRuuning == FALSE)
        {
            printf("The limits of attempts was exceeded. App is closing...\n");
        }
        return 0;
}
