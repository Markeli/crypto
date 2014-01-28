#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <common.h>


#define BUFSIZE 1024
#define PORT 8547

char userName[PARAMETRS_LENGTH];

static void ConnectRequest(int *sockfd, struct sockaddr_in *serverAddres);
static void SendRecieve(int i, int socketFD, char userName[PARAMETRS_LENGTH]);

int RunClient(char _userName[PARAMETRS_LENGTH])
{
    int socketFD, fdMax, i;
    struct sockaddr_in serverAddres;
    fd_set master;
    fd_set readFDS;
    strcpy(userName, _userName);
    ConnectRequest(&socketFD, &serverAddres);
    FD_ZERO(&master);
    FD_ZERO(&readFDS);
    FD_SET(0, &master);
    FD_SET(socketFD, &master);
    fdMax = socketFD;

    while(1)
    {
        readFDS = master;
        if(select(fdMax+1, &readFDS, NULL, NULL, NULL) == -1)
        {
            perror("select");
            exit(4);
        }

        for(i=0; i <= fdMax; i++ )
        {
            if(FD_ISSET(i, &readFDS))
            {
                SendRecieve(i, socketFD, userName);
            }
        }
    }
    printf("Client quited\n");
    close(socketFD);
    return 0;
}

static void ConnectRequest(int *socketFD, struct sockaddr_in *serverAddres)
{
    char buffer[BUFSIZE];
    int recievedBytesCount;
    if ((*socketFD = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Socket");
        exit(1);
    }
    serverAddres->sin_family = AF_INET;
    serverAddres->sin_port = htons(PORT);
    serverAddres->sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(serverAddres->sin_zero, '\0', sizeof serverAddres->sin_zero);

    if(connect(*socketFD, (struct sockaddr *)serverAddres, sizeof(struct sockaddr)) == -1)
    {
        perror("connect");
        exit(1);
    }
    strcpy(buffer, userName);

    if (send(*socketFD, buffer, BUFSIZE, 0) <= 0)
    {
        perror("connect");
        exit(1);
    }
    if ((recievedBytesCount = recv(*socketFD, buffer, BUFSIZE, 0)) <= 0)
    {
        FixRecievingError(recievedBytesCount, *socketFD, "Connetion error occured.\n");
        exit(1);
    }
    else
    {
        if (strcmp(buffer, "wellcome") == 0)
        {
            printf("Connetion is succesfull.\n");
        }
        else
        {
            if (strcmp(buffer, "sorry") == 0)
            {
                printf("This username is already taken. Choose antoher and try again.\n");
                close(*socketFD);
                exit(1);
            }
            else
            {
                if (strcmp(buffer, "admin") == 0)
                {
                    printf("This username belongs to admin. If you are admin, enter password:.\n");
                    GetPassword(buffer);
                    if (send(*socketFD, buffer, BUFSIZE, 0) <= 0)
                    {
                        perror("connect");
                        exit(1);
                    }
                    if ((recievedBytesCount = recv(*socketFD, buffer, BUFSIZE, 0)) <= 0)
                    {
                        FixRecievingError(recievedBytesCount, *socketFD, "Connetion error occured.\n");
                        exit(1);
                    }
                    else
                    {
                        if (strcmp(buffer, "wellcome") == 0)
                        {
                            printf("Connetion is succesfull.\n");
                        }
                        else
                        {
                            printf("Incorrect password. You are not an admin. App is closing...\n");
                            close(*socketFD);
                            exit(1);
                        }
                    }
                }
            }
        }
    }
}

static void SendRecieve(int i, int socketFD, char userName[PARAMETRS_LENGTH])
{
    char tempBuf[BUFSIZE];
    char ioBuf[BUFSIZE];
    int bytesRecieved;

    if (i == 0)
    {
        fgets(tempBuf, BUFSIZE, stdin);
        if (strcmp(tempBuf , "quit\n") == 0)
        {
            exit(0);
        }
        else
        {
            strcpy(ioBuf, userName);
            strcat(ioBuf, ":\n");
            strcat(ioBuf, tempBuf);
            send(socketFD, ioBuf, strlen(ioBuf), 0);
        }
    }
    else
    {
        bytesRecieved = recv(socketFD, ioBuf, BUFSIZE, 0);
        if (bytesRecieved <=0)
        {
            FixRecievingError(bytesRecieved, &socketFD, "Recieving error. Maybe server closed. App is closing...\n");
            exit(1);
        }
        else
        {
            ioBuf[bytesRecieved] = '\0';
            printf("%s\n" , ioBuf);
            fflush(stdout);
        }
    }
}
