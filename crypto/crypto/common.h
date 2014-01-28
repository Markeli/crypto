#ifndef COMMON_H
#define COMMON_H

#define TRUE 1
#define FALSE 0
#define PARAMETRS_LENGTH 20
#define ERRORS_LIMIT 3

void GetUserName(char userName[PARAMETRS_LENGTH]);
void GetPassword(char password[PARAMETRS_LENGTH]);
void FixRecievingError(int recievedBytesCount, int *socketFD, char* errorMessage);

#endif
