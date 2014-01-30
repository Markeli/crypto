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
#include <termios.h>
#include <curses.h>
#include <pthread.h>
#include <openssl/aes.h>
#include <common.h>

char userName[PARAMETRS_LENGTH];
WINDOW *top;
WINDOW *bottom;
int line=1; // Line position of top
int input=1; // Line position of top
int maxx,maxy; // Screen dimensions
pthread_mutex_t mutexsum = PTHREAD_MUTEX_INITIALIZER;

unsigned char key[32];
AES_KEY encKey, decKey;

static void ConnectRequest(int *sockfd, struct sockaddr_in *serverAddres);
static void SendRecieve(int i, int socketFD, char userName[PARAMETRS_LENGTH]);

static void *SendMessage(int socketFD);

static void *RecieveMessage(int socketFD);

int RunClient(char _userName[PARAMETRS_LENGTH])
{
    int socketFD, fdMax, i;
    struct sockaddr_in serverAddres;
    fd_set master;
    fd_set readFDS;
    pthread_t threads[2];
    void *status;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    strcpy(userName, _userName);
    ConnectRequest(&socketFD, &serverAddres);
    FD_ZERO(&master);
    FD_ZERO(&readFDS);
    FD_SET(0, &master);
    FD_SET(socketFD, &master);
    fdMax = socketFD;

    initscr();
    getmaxyx(stdscr,maxy,maxx);

    top = newwin(maxy/2,maxx,0,0);
    bottom= newwin(maxy/2,maxx,maxy/2,0);

    scrollok(top,TRUE);
    scrollok(bottom,TRUE);
    box(top,'|','=');

    wsetscrreg(top,1,maxy/2-2);
    wsetscrreg(bottom,1,maxy/2-2);

    wrefresh(top);
    wrefresh(bottom);


    while(1)
    {
        wrefresh(top);
        wrefresh(bottom);
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
    return 0;
}

static void ConnectRequest(int *socketFD, struct sockaddr_in *serverAddres)
{
    unsigned char buffer[BUFSIZE];
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
    /*Getting aes key*/
    if ((recievedBytesCount = recv(*socketFD, key, 32, 0)) <= 0)
    {
        FixRecievingError(recievedBytesCount, *socketFD, "Connetion error occured. Can't receive aes key.\n");
        exit(1);
    }

    AES_set_encrypt_key(key, 128, &encKey);
    AES_set_decrypt_key(key, 128, &decKey);
    /*Sending username to server*/
    strcpy(buffer, userName);
    AES_encrypt(buffer, buffer, &encKey);
    if (send(*socketFD, buffer, BUFSIZE, 0) <= 0)
    {
        perror("connect username");
        exit(1);
    }
    if ((recievedBytesCount = recv(*socketFD, buffer, BUFSIZE, 0)) <= 0)
    {
        FixRecievingError(recievedBytesCount, *socketFD, "Connetion error occured.\n");
        exit(1);
    }
    else
    {
        AES_decrypt(buffer, buffer, &decKey);
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
                    GetPassword(buffer, PASS_CLIENT);
                    AES_encrypt(buffer, buffer, &encKey);
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
                        AES_decrypt(buffer, buffer, &decKey);
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
    unsigned char tempBuf[BUFSIZE];
    unsigned char ioBuf[BUFSIZE];
    int bytesRecieved;

    if (i == 0)
    {

        mvwgetstr(bottom,input,2,tempBuf);
        if (strcmp(tempBuf , "quit") == 0)
        {
            endwin();
            printf("Client quited\n");
            close(socketFD);
            exit(0);
        }
        else
        {
            strcpy(ioBuf, userName);
            strcat(ioBuf, ": ");
            strcat(ioBuf, tempBuf);

            AES_encrypt(ioBuf,ioBuf, &encKey);
            send(socketFD, ioBuf, strlen(ioBuf), 0);

            if(line!=maxy/2-2)
            {
                line++;
            }
            else
            {
                scroll(top);
            }
            wclear(bottom);
        }
    }
    else
    {
        bytesRecieved = recv(socketFD, ioBuf, BUFSIZE, 0);

        if (bytesRecieved <=0)
        {
            endwin();
            FixRecievingError(bytesRecieved, &socketFD, "Recieving error. Maybe server closed. App is closing...\n");
            exit(1);
        }
        else
        {
            AES_decrypt(ioBuf,ioBuf,&decKey);
            ioBuf[bytesRecieved] = '\0';
            mvwprintw(top,line,3,ioBuf);
            if(line!=maxy/2-2)
            {
                line++;
            }
            else
            {

                scroll(top);
            }
        }
    }
    wrefresh(top);
    wrefresh(bottom);
}
