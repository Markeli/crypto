#ifndef CIPHER_H
#define CIPHER_H
#include <openssl/evp.h>

void Generate_AES_256_KEY(char key[32]);

int DoCrypt();

#endif // CIPHER_H
