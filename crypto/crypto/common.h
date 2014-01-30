#ifndef COMMON_H
#define COMMON_H

#define TRUE 1
#define FALSE 0
#define PARAMETRS_LENGTH 20
#define ERRORS_LIMIT 3
#define PASS_CLIENT 0
#define PASS_SERVER 1

#define BUFSIZE 1024
#define PORT 8547

void GetUserName(char userName[PARAMETRS_LENGTH]);
void GetPassword(char password[PARAMETRS_LENGTH], int passwordOwner);
void FixRecievingError(int recievedBytesCount, int* socketFD, char* errorMessage);

#endif
