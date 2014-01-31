#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <openssl/evp.h>
#include <openssl/aes.h>

#include <common.h>
#include <list.h>
#include <cipher.h>

char adminsName[PARAMETRS_LENGTH];
char adminsPass[PARAMETRS_LENGTH];
int adminsSocketFD;
List *clientList;

unsigned char* AESKey;
EVP_CIPHER_CTX ctx;
AES_KEY encKey,decKey;

static void ConnectRequest(int *socketFD, struct sockaddr_in *myAddres);
static void ConnectionAccept(fd_set *master, int *fdMax, int socketFD, struct sockaddr_in *clientAddres);
static void DenialOfAccepting(int *newSocketFD);
static void AcceptClient(int *newSocketFD, fd_set *master, int *fdMax, struct sockaddr_in *clientAddres);
static void ReSend(int activeSocket, fd_set *master, int serverSocketFD, int fdMax);
static int SearchControlMessage(unsigned char recievedBuf[BUFSIZE]);
static void SendToAll(int currentSocketFD, int serverSocketFD, int recievedBytesCount, unsigned char *recievedBuf, fd_set *master);
void CreateAESContext();

int RunServer(char userName[PARAMETRS_LENGTH])
{
    fd_set master;
    fd_set readFDS;
    int fdMax, activeSocket;
    int serverSocketFD= 0;
    struct sockaddr_in myAddres, clientAddres;

    clientList = CreateList();
    adminsSocketFD = -1;
    GetPassword(adminsPass, PASS_SERVER);
    printf("Password have been saved.\n");

    printf("Generating AES key...  ");
    AESKey = Generate_AES_256_KEY();
    AES_set_encrypt_key(AESKey, 128, &encKey);
    AES_set_decrypt_key(AESKey, 128, &decKey);
    printf("done\n");
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

        for (activeSocket = 0; activeSocket <= fdMax; activeSocket++)
        {
            if (FD_ISSET(activeSocket, &readFDS))
            {
                if (activeSocket == serverSocketFD)
                {
                    ConnectionAccept(&master, &fdMax, serverSocketFD, &clientAddres);
                }
                else
                {
                    ReSend(activeSocket, &master, serverSocketFD, fdMax);
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
    int receivedBytesCount, keySize, encSize;
    socklen_t addresLength;
    int clientSocketFD;
    unsigned char recievedBuffer[BUFSIZE], sendBuffer[BUFSIZE];
    unsigned char* publicKeyBuffer;
    RSA *RSAPublicKey;
    addresLength = sizeof(struct sockaddr_in);
    printf("\nCreating new conection...\n");
    if((clientSocketFD = accept(serverSocketFD, (struct sockaddr *)clientAddres, &addresLength)) == -1)
    {
        perror("accept");
        exit(1);
    }
    else
    {
        printf("Getting public RSA key...  ");
        publicKeyBuffer = malloc(RSA_PUBLIC_KEY_LENGTH+1);
        if ((receivedBytesCount = recv(clientSocketFD, publicKeyBuffer, RSA_PUBLIC_KEY_LENGTH, 0)) <= 0)
        {
            printf("\nError. Recieve %d bytes.\n", receivedBytesCount);
            FixReceivingError(receivedBytesCount, &clientSocketFD,"Getting public key error.\n");
            exit(1);
        }
        printf("done\n");

        printf("Decoding RSA public key...  ");
        GetPublicKeyFromBuffer(&RSAPublicKey, publicKeyBuffer, RSA_PUBLIC_KEY_LENGTH);
        if (RSAPublicKey == NULL)
        {
            perror("\nDecoding public key error.");
            exit(1);
        }
        printf("done\n");
        free(publicKeyBuffer);

        /*Sending AES_256 key*/
        printf("Encrypting AES 256 key...");
        printf("\n%s\n", AESKey);
        encSize = EncryptSimmetricKey(AESKey, sendBuffer, AES_KEY_LENGTH, RSAPublicKey );
        printf("\n%s\n", AESKey);
        printf("  done\n");
        printf("Sending cipher AES 256 key...");
        keySize = RSA_size(RSAPublicKey);
        if (send(clientSocketFD, sendBuffer, keySize, 0) == -1)
        {
            perror("send");
            exit(1);
        }
        printf("done\n");

        /*Getting clients username*/;
        if ((receivedBytesCount = recv(clientSocketFD, recievedBuffer, BUFSIZE, 0)) <= 0)
        {
            FixReceivingError(receivedBytesCount, &clientSocketFD,"Connetion error occured.\n");
        }
        else
        {
            AES_decrypt(recievedBuffer, recievedBuffer, &decKey);
            /*Check uniq username in clientList*/
            if (InsertItemUniq(clientList, clientSocketFD, recievedBuffer) == TRUE)
            {
                /*Check if username belongs to admin*/
                if (strcmp(recievedBuffer, adminsName) == 0)
                {
                    /*Requesting for getting admins password*/
                    strcpy(sendBuffer, "admin");
                    AES_encrypt(sendBuffer,sendBuffer, &encKey);
                    if (send(clientSocketFD, sendBuffer, BUFSIZE, 0) == -1)
                    {
                        perror("send");
                    }
                    /*Getting admins password from client*/
                    if ((receivedBytesCount = recv(clientSocketFD, recievedBuffer, BUFSIZE, 0)) <= 0)
                    {
                        FixReceivingError(receivedBytesCount, &clientSocketFD,"Connetion error occured.\n");
                    }
                    else
                    {
                        AES_decrypt(recievedBuffer,recievedBuffer, &decKey);
                        if (strcmp(recievedBuffer, adminsPass) == 0)
                        {
                            AcceptClient(&clientSocketFD, master, fdMax, clientAddres);
                            adminsSocketFD = clientSocketFD;
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
    unsigned char sendBuffer[BUFSIZE];
    strcpy(sendBuffer, "welcome");
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
    //strcpy(sendBuffer, recievedBuffer);
    //strcpy(sendBuffer, " joined to the chat!\n");
    //SendToAll(socketFD, socketFD, socketFD, sendBuffer, BUFSIZE, master );
}

static void DenialOfAccepting(int *newSocketFD)
{
    unsigned char sendBuffer[BUFSIZE];
    strcpy(sendBuffer, "sorry");
    if (send(*newSocketFD, sendBuffer, BUFSIZE, 0) == -1)
    {
        perror("send");
    }
    close(*newSocketFD);
}

static void ReSend(int activeSocket, fd_set *master, int serverSocketFD, int fdMax)
{
    int recievedBytesCount, clientSocket;
    unsigned char recievedBuf[BUFSIZE];

    if ((recievedBytesCount = recv(activeSocket, recievedBuf, BUFSIZE, 0)) <= 0)
    {
        if (recievedBytesCount == 0)
        {
            printf("socket %d hung up\n", activeSocket);
        }
        else
        {
            perror("recv");
        }
        if (activeSocket == adminsSocketFD)
        {
            adminsSocketFD = -1;
        }
        close(activeSocket);
        DeleteItem(clientList, activeSocket);
        FD_CLR(activeSocket, master);
    }
    else
    {
        if (activeSocket == adminsSocketFD)
        {
            SearchControlMessage(recievedBuf);
        }
        else
        {
            for(clientSocket = 0; clientSocket <= fdMax; clientSocket++)
            {
                SendToAll(clientSocket, serverSocketFD, recievedBytesCount, recievedBuf, master );
            }
        }
    }
}

static int SearchControlMessage(unsigned char recievedBuf[BUFSIZE])
{
    if (!strstr(recievedBuf , "server -close"))
    {
        exit(0);
    }
    return 0;
}

static void SendToAll(int currentSocketFD, int serverSocketFD, int recievedBytesCount, unsigned char *recievedBuf, fd_set *master)
{
    if (FD_ISSET(currentSocketFD, master))
    {
        if (currentSocketFD != serverSocketFD)
        {
            if (send(currentSocketFD, recievedBuf, recievedBytesCount, 0) == -1)
            {
                perror("send");
            }
        }
    }
}
