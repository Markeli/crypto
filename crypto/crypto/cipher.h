#ifndef CIPHER_H
#define CIPHER_H
#include <openssl/pem.h>

#define RSA_KEY_LENGTH 2048
#define RSA_PUBLIC_KEY_LENGTH 426
#define AES_KEY_LENGTH 32
#define PRIVATE "./privat.key"

unsigned char* Generate_AES_256_KEY();

RSA* Generate_RSA_Keys();

unsigned char* GetPublicKeyFromRSA(RSA *rsa, int *publicKeyLength);

void GetPublicKeyFromBuffer(RSA **rsa, unsigned char* publicKey, int keyLength);

void GetPrivateKey(RSA *privateKey);
//void GetPrivateKey(RSA *keyPair, RSA **privateKeyRSA);

int EncryptSimmetricKey(unsigned char *key, unsigned char *sendBuffer, int size, RSA *rsa);

unsigned char* DecryptSimmetricKey(unsigned char* ciperKey, RSA* rsa);

#endif // CIPHER_H
