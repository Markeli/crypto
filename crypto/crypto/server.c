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
#include <list.h>

#define PORT 8547
#define BUFSIZE 1024

char adminsName[PARAMETRS_LENGTH];
char adminsPass[PARAMETRS_LENGTH];
List *clientList;

static void ConnectRequest(int *socketFD, struct sockaddr_in *myAddres);
static void ConnectionAccept(fd_set *master, int *fdMax, int socketFD, struct sockaddr_in *clientAddres);
static void DenialOfAccepting(int *newSocketFD);
static void AcceptClient(int *newSocketFD, fd_set *master, int *fdMax, struct sockaddr_in *clientAddres);
static void ReSend(int i, fd_set *master, int socketFD, int fdMax);
static int SearchControlMessage(char recievedBuf[BUFSIZE]);
static void SendToAll(int j, int i, int socketFD, int recievedBytesCount, char *recievedBuf, fd_set *master);



int RunServer(char userName[PARAMETRS_LENGTH])
{
    fd_set master;
    fd_set readFDS;
    int fdMax, i;
    int serverSocketFD= 0;
    struct sockaddr_in myAddres, clientAddres;
    clientList = CreateList();

    GetPassword(adminsPass);
    printf("Password have been saved.\n");
    FD_ZERO(&master);
    FD_ZERO(&readFDS);
    ConnectRequest(&serverSocketFD, &myAddres);
    FD_SET(serverSocketFD, &master);
    fdMax = serverSocketFD;
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
                if (i == serverSocketFD)
                {
                    ConnectionAccept(&master, &fdMax, serverSocketFD, &clientAddres);
                }
                else
                {
                    ReSend(i, &master, serverSocketFD, fdMax);
                }
            }
        }
    }
    return 0;
}

static void ConnectRequest(int *serverSocketFD, struct sockaddr_in *myAddres)
{
    int yes = 1;
    if ((*serverSocketFD = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Socket");
        exit(1);
    }
    myAddres->sin_family = AF_INET;
    myAddres->sin_port = htons(PORT);
    myAddres->sin_addr.s_addr = INADDR_ANY;
    memset(myAddres->sin_zero, '\0', sizeof myAddres->sin_zero);

    if (setsockopt(*serverSocketFD, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
    {
        perror("setsockopt");
        exit(1);
    }

    if (bind(*serverSocketFD, (struct sockaddr *)myAddres, sizeof(struct sockaddr)) == -1)
    {
        perror("Unable to bind");
        exit(1);
    }
    if (listen(*serverSocketFD, 10) == -1)
    {
        perror("listen");
        exit(1);
    }
    printf("\nCryptoServer waiting for client on port %d\n", PORT);
    fflush(stdout);
}

/*Accepting new client*/
static void ConnectionAccept(fd_set *master, int *fdMax, int serverSocketFD, struct sockaddr_in *clientAddres)
{
    int recievedBytesCount;
    socklen_t addresLength;
    int clientSocketFD;
    char recievedBuffer[BUFSIZE], sendBuffer[BUFSIZE];

    addresLength = sizeof(struct sockaddr_in);
    if((clientSocketFD = accept(serverSocketFD, (struct sockaddr *)clientAddres, &addresLength)) == -1)
    {
        perror("accept");
        exit(1);
    }
    else
    {
        /*Getting clients username*/
        if ((recievedBytesCount = recv(clientSocketFD, recievedBuffer, BUFSIZE, 0)) <= 0)
        {
            FixRecievingError(recievedBytesCount, &clientSocketFD,"Connetion error occured.\n");
        }
        else
        {
            /*Check uniq username in clientList*/
            if (InsertItemUniq(clientList, clientSocketFD, recievedBuffer) == TRUE)
            {
                /*Check if username belongs to admin*/
                if (strcmp(recievedBuffer, adminsName) == 0)
                {
                    strcpy(sendBuffer, "admin");
                    /*Requesting for getting admins password*/
                    if (send(clientSocketFD, sendBuffer, BUFSIZE, 0) == -1)
                    {
                        perror("send");
                    }
                    /*Getting admins password from client*/
                    if ((recievedBytesCount = recv(clientSocketFD, recievedBuffer, BUFSIZE, 0)) <= 0)
                    {
                        FixRecievingError(recievedBytesCount, &clientSocketFD,"Connetion error occured.\n");
                    }
                    else
                    {
                        if (strcmp(recievedBuffer, adminsPass) == 0)
                        {
                            AcceptClient(&clientSocketFD, master, fdMax, clientAddres);
                        }
                        else
                        {
                            DenialOfAccepting(&clientSocketFD);
                        }
                    }

                }
                else
                {
                    AcceptClient(&clientSocketFD, master, fdMax, clientAddres);
                }
            }
            else
            {
                DenialOfAccepting(&clientSocketFD);
            }
        }
    }
}

static void AcceptClient(int *newSocketFD, fd_set *master, int *fdMax, struct sockaddr_in *clientAddres)
{
    char sendBuffer[BUFSIZE];
    strcpy(sendBuffer, "wellcome");
    if (send(*newSocketFD, sendBuffer, BUFSIZE, 0) == -1)
    {
        perror("send");
    }
    FD_SET(*newSocketFD, master);
    if(*newSocketFD > *fdMax)
    {
        *fdMax = *newSocketFD;
    }
    printf("New connection from %s on port %d \n",inet_ntoa(clientAddres->sin_addr), ntohs(clientAddres->sin_port));
    PrintList(clientList);
    //strcpy(sendBuffer, recievedBuffer);
    //strcpy(sendBuffer, " joined to the chat!\n");
    //SendToAll(socketFD, socketFD, socketFD, sendBuffer, BUFSIZE, master );
}

static void DenialOfAccepting(int *newSocketFD)
{
    char sendBuffer[BUFSIZE];
    strcpy(sendBuffer, "sorry");
    if (send(*newSocketFD, sendBuffer, BUFSIZE, 0) == -1)
    {
        perror("send");
    }
    close(*newSocketFD);
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
        DeleteItem(clientList, i);
        close(i);
        FD_CLR(i, master);
    }
    else
    {
        //SearchControlMessage(recievedBuf);
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
