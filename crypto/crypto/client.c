#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <common.h>
#include <string.h>

#define BUFSIZE 1024
#define PORT 8547

static void ConnectRequest(int *sockfd, struct sockaddr_in *serverAddres);
static void SendRecieve(int i, int socketFD, char userName[USER_NAME_LENGTH]);

int RunClient(char userName[USER_NAME_LENGTH])
{
    int socketFD, fdMax, i;
    struct sockaddr_in serverAddres;
    fd_set master;
    fd_set readFDS;

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
    printf("client-quited\n");
    close(socketFD);
    return 0;
}

static void ConnectRequest(int *sockfd, struct sockaddr_in *serverAddres)
{
    if ((*sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Socket");
        exit(1);
    }
    serverAddres->sin_family = AF_INET;
    serverAddres->sin_port = htons(PORT);
    serverAddres->sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(serverAddres->sin_zero, '\0', sizeof serverAddres->sin_zero);

    if(connect(*sockfd, (struct sockaddr *)serverAddres, sizeof(struct sockaddr)) == -1)
    {
        perror("connect");
        exit(1);
    }
    printf("Wellcome to room.");
}

static void SendRecieve(int i, int socketFD, char userName[USER_NAME_LENGTH])
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
            strcat(ioBuf, userName);
            strcat(ioBuf, ": ");
            strcat(ioBuf, tempBuf);
            send(socketFD, ioBuf, strlen(ioBuf), 0);
        }
    }
    else
    {
        bytesRecieved = recv(socketFD, ioBuf, BUFSIZE, 0);
        ioBuf[bytesRecieved] = '\0';
        printf("%s\n" , ioBuf);
        fflush(stdout);
    }
}
