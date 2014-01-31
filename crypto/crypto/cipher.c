#include <common.h>
#include <cipher.h>
#include <openssl/rand.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <stdio.h>
#include <string.h>

unsigned char* Generate_AES_256_KEY()
{    
    unsigned char* key;
    key = malloc(AES_KEY_LENGTH);
    if (RAND_bytes(key, AES_KEY_LENGTH) == 0)
    {
        perror("AES key genretion");
        exit(1);
    }
    return key;
}

RSA* Generate_RSA_Keys()
{
    unsigned long exp = RSA_F4;
    RSA * rsa;
    FILE *privateKeyFile;
    unsigned char* kstr ="XXXX";
    rsa = RSA_generate_key(RSA_KEY_LENGTH,exp,NULL,NULL);

    privateKeyFile = fopen(PRIVATE, "w");

    PEM_write_RSAPrivateKey(privateKeyFile, rsa, EVP_des_ede3_cbc(), kstr, strlen(kstr), NULL, "hello");
    fclose(privateKeyFile);
    return rsa;
}

//void GetPrivateKey(RSA *keyPair, RSA **privateKeyRSA)
void GetPrivateKey(RSA *privateKey)
{
    RSA *rsa;
    FILE *privateKeyFile;
    unsigned char* kstr ="XXXXXX";
    rsa = RSA_new();
    privateKeyFile = fopen(PRIVATE, "r");
    OpenSSL_add_all_algorithms();
    privateKey = PEM_read_RSAPrivateKey(privateKeyFile, &rsa, NULL, kstr);
    fclose(privateKeyFile);
    /*
    unsigned char *privateKey;
    int keyLength;
    BIO *privateKeyBIO = BIO_new(BIO_s_mem());

    PEM_write_bio_RSAPrivateKey(privateKeyBIO, keyPair, NULL, NULL, NULL, NULL, NULL);
    //PEM_write_bio_RSAPublicKey(privateKeyBIO, keyPair);
    keyLength = BIO_pending(privateKeyBIO);
    privateKey = malloc(keyLength+ 1);
    BIO_read(privateKeyBIO, privateKey, keyLength);

    privateKeyBIO = BIO_new_mem_buf((void*)privateKey, keyLength);
   // PEM_read_bio_RSAPublicKey(bio, privateKeyRSA, 0, NULL);
    PEM_write_bio_RSAPrivateKey(privateKeyBIO, privateKeyRSA, NULL, NULL, NULL, NULL, NULL);*/
}

unsigned char* GetPublicKeyFromRSA(RSA *rsa, int *publicKeyLength)
{
    unsigned char *publicKey;
    int keyLength;
    BIO *publicKeyBIO = BIO_new(BIO_s_mem());
    PEM_write_bio_RSAPublicKey(publicKeyBIO, rsa);
    keyLength = BIO_pending(publicKeyBIO);
    publicKey = malloc(keyLength+ 1);
    BIO_read(publicKeyBIO, publicKey, keyLength);
    *publicKeyLength = keyLength;
    return publicKey;
}

void GetPublicKeyFromBuffer(RSA **rsa, unsigned char* publicKey, int keyLength)
{
    BIO *bio;
    bio = BIO_new_mem_buf((void*)publicKey, keyLength);
    PEM_read_bio_RSAPublicKey(bio, rsa, 0, NULL);
}

int EncryptSimmetricKey(unsigned char *key, unsigned char *sendBuffer, int size, RSA *rsa)
{
    return RSA_public_encrypt(size, key, sendBuffer, rsa, RSA_PKCS1_PADDING );
}

unsigned char* DecryptSimmetricKey(unsigned char* ciperKey, RSA* rsa)
{
    unsigned char* simmetrucKey = malloc(RSA_size(rsa)); // RSA_size(rsa) is the modulus
    if( simmetrucKey == NULL)
        return NULL;
    int k = RSA_private_decrypt(RSA_size(rsa), ciperKey, simmetrucKey, rsa, RSA_PKCS1_PADDING);
    if( k == -1)
        return NULL;
    return simmetrucKey;
}
