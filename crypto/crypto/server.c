#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORT 8547
#define BUFSIZE 1024

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

static void SendRecieve(int i, fd_set *master, int socketFD, int fdMax)
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
    //	printf("%s\n", recv_buf);
        for(j = 0; j <= fdMax; j++)
        {
            SendToAll(j, i, socketFD, recievedBytesCount, recievedBuf, master );
        }
    }
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
        printf("new connection from %s on port %d \n",inet_ntoa(clientAddres->sin_addr), ntohs(clientAddres->sin_port));
    }
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

int RunServer()
{
    fd_set master;
    fd_set readFDS;
    int fdMax, i;
    int socketFD= 0;
    struct sockaddr_in myAddres, clientAddres;
    FD_ZERO(&master);
    FD_ZERO(&readFDS);
    ConnectRequest(&socketFD, &myAddres);
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

        for (i = 0; i <= fdMax; i++)
        {
            if (FD_ISSET(i, &readFDS))
            {
                if (i == socketFD)
                    ConnectionAccept(&master, &fdMax, socketFD, &clientAddres);
                else
                    SendRecieve(i, &master, socketFD, fdMax);
            }
        }
    }
    return 0;
}
