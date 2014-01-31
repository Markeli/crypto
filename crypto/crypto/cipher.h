#ifndef CIPHER_H
#define CIPHER_H
#include <openssl/pem.h>

#define RSA_KEY_LENGTH 2048
#define PRIVAT "./privat.key"
#define PUBLIC "./public.key"
#define RSA_PUBLIC_KEY_LENGTH 426

void Generate_AES_256_KEY(unsigned char key[32]);

RSA* Generate_RSA_Keys();

unsigned char *DER_encode_RSA_public(RSA *rsa, int *len);

RSA *DER_decode_RSA_public(unsigned char *buf, long len);

void GetPublicKeyString(char buffer[RSA_KEY_LENGTH]);

unsigned char* GetPublicKeyFromRSA(RSA *rsa, int *publicKeyLength);

RSA* GetPublicKeyFromBuffer(char* publicKey, int keyLength);

RSA* GetPrivateKey();

int EncryptSimmetricKey(unsigned char *key, unsigned char *sendBuffer, int size, RSA *rsa);

unsigned char* DecryptSimmetricKey(char* ciperKey, RSA* rsa);
#endif // CIPHER_H
