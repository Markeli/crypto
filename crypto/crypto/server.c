#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <common.h>

#define PORT 8547
#define BUFSIZE 1024

char adminsName[PARAMETRS_LENGTH];
char adminsPass[PARAMETRS_LENGTH];

static void ConnectRequest(int *socketFD, struct sockaddr_in *myAddres);
static void ConnectionAccept(fd_set *master, int *fdMax, int socketFD, struct sockaddr_in *clientAddres);
static void ReSend(int i, fd_set *master, int socketFD, int fdMax);
static int SearchControlMessage(char recievedBuf[BUFSIZE]);
static void SendToAll(int j, int i, int socketFD, int recievedBytesCount, char *recievedBuf, fd_set *master);


int RunServer(char userName[PARAMETRS_LENGTH])
{
    fd_set master;
    fd_set readFDS;
    int fdMax, i;
    int socketFD= 0;
    struct sockaddr_in myAddres, clientAddres;

    GetParametr(adminsPass, "admins password\0");
    printf("Password have been saved.\n");
    FD_ZERO(&master);
    FD_ZERO(&readFDS);
    ConnectRequest(&socketFD, &myAddres);
    FD_SET(socketFD, &master);
    fdMax = socketFD;
    strcpy(adminsName,userName);
    while(1)
    {
        readFDS = master;
        if(select(fdMax+1, &readFDS, NULL, NULL, NULL) == -1)
        {
            perror("select");
            exit(4);
        }

        for (i = 0; i <= fdMax; i++)
        {
            if (FD_ISSET(i, &readFDS))
            {
                if (i == socketFD)
                    ConnectionAccept(&master, &fdMax, socketFD, &clientAddres);
                else
                    ReSend(i, &master, socketFD, fdMax);
            }
        }
    }
    return 0;
}

static void ConnectRequest(int *socketFD, struct sockaddr_in *myAddres)
{
    int yes = 1;
    if ((*socketFD = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Socket");
        exit(1);
    }

    myAddres->sin_family = AF_INET;
    myAddres->sin_port = htons(PORT);
    myAddres->sin_addr.s_addr = INADDR_ANY;
    memset(myAddres->sin_zero, '\0', sizeof myAddres->sin_zero);

    if (setsockopt(*socketFD, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
    {
        perror("setsockopt");
        exit(1);
    }

    if (bind(*socketFD, (struct sockaddr *)myAddres, sizeof(struct sockaddr)) == -1)
    {
        perror("Unable to bind");
        exit(1);
    }
    if (listen(*socketFD, 10) == -1)
    {
        perror("listen");
        exit(1);
    }
    printf("\nCryptoServer waiting for client on port %d\n", PORT);
    fflush(stdout);
}

static void ConnectionAccept(fd_set *master, int *fdMax, int socketFD, struct sockaddr_in *clientAddres)
{
    socklen_t addresLength;
    int newSocketFD;

    addresLength = sizeof(struct sockaddr_in);
    if((newSocketFD = accept(socketFD, (struct sockaddr *)clientAddres, &addresLength)) == -1)
    {
        perror("accept");
        exit(1);
    }
    else
    {
        FD_SET(newSocketFD, master);
        if(newSocketFD > *fdMax){
            *fdMax = newSocketFD;
        }
        printf("New connection from %s on port %d \n",inet_ntoa(clientAddres->sin_addr), ntohs(clientAddres->sin_port));
    }
}

static void ReSend(int i, fd_set *master, int socketFD, int fdMax)
{
    int recievedBytesCount, j;
    char recievedBuf[BUFSIZE];

    if ((recievedBytesCount = recv(i, recievedBuf, BUFSIZE, 0)) <= 0)
    {
        if (recievedBytesCount == 0)
        {
            printf("socket %d hung up\n", i);
        }
        else
        {
            perror("recv");
        }
        close(i);
        FD_CLR(i, master);
    }
    else
    {
        SearchControlMessage(recievedBuf);
        for(j = 0; j <= fdMax; j++)
        {
            SendToAll(j, i, socketFD, recievedBytesCount, recievedBuf, master );
        }
    }
}

static int SearchControlMessage(char recievedBuf[BUFSIZE])
{
    char tempBuf[BUFSIZE];
    strcpy(tempBuf, adminsName);
    strcat(tempBuf, ":\nserver close\n");
    printf("rec\n%s\ntemp\n%s\n", recievedBuf, tempBuf);

    if (!strstr(recievedBuf , tempBuf))
    {
        exit(0);
    }
}

static void SendToAll(int j, int i, int socketFD, int recievedBytesCount, char *recievedBuf, fd_set *master)
{
    if (FD_ISSET(j, master))
    {
        if (j != socketFD && j != i)
        {
            if (send(j, recievedBuf, recievedBytesCount, 0) == -1)
            {
                perror("send");
            }
        }
    }
}
