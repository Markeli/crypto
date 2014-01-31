#include <common.h>
#include <cipher.h>
#include <openssl/rand.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <stdio.h>

void Generate_AES_256_KEY(unsigned char key[32])
{
    if (RAND_bytes(key, 32) == 0)
    {
        perror("AES key genretion");
        exit(1);
    }
}

RSA* Generate_RSA_Keys()
{
    unsigned long exp = RSA_F4;
    RSA * rsa;
    rsa = RSA_generate_key(RSA_KEY_LENGTH,exp,NULL,NULL);
    return rsa;
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
    publicKey[keyLength] = '\0';
    *publicKeyLength = keyLength;
    return publicKey;
}

RSA* GetPublicKeyFromBuffer(char* publicKey, int keyLength)
{
    BIO *bio;
    RSA *rsa;
    printf("\nBIO...");
    bio = BIO_new_mem_buf((void*)publicKey, keyLength);
    printf("PEM...");
    PEM_read_bio_RSAPublicKey(bio, &rsa, 0, NULL);
    printf("return...\n");
    return rsa;
}

unsigned char *DER_encode_RSA_public(RSA *rsa, int *len)
{
  unsigned char *buf, *next;

  *len = i2d_RSAPublicKey(rsa, 0);
  if (!(buf = next = (unsigned char *)malloc(*len))) return 0;
  i2d_RSAPublicKey(rsa, &next); /* If we use buf here, return buf; becomes wrong */
  return buf;
}

RSA *DER_decode_RSA_public(unsigned char *buf, long len)
{
  return d2i_RSAPublicKey(0, &buf, len);
}

void GetPublicKeyString(char buffer[RSA_KEY_LENGTH])
{
    int i;
    char *buf;
    FILE *fp = fopen("key_pub", "r");
    fgets(buf, RSA_KEY_LENGTH, fp);
    fread(buf, 1, RSA_KEY_LENGTH, fp);
    close(fp);
    for (i=0; i<RSA_KEY_LENGTH; ++i)
        printf("%s", buf[i]);
    printf("\n");
    //i2d_RSAPublicKey();
}
/*
RSA* GetPublicKey()
{
    RSA *rsa = RSA_new();
    FILE *fp = fopen("key_pub","r");

    if( fp == NULL)
        return NULL;

    RSA *rs = PEM_read_RSAPublicKey(fp, &rsa, NULL,NULL);

    return rs;
}*/

RSA* GetPrivateKey()
{
    OpenSSL_add_all_algorithms();
    RSA *rsa = RSA_new();
    FILE *fp = fopen("key_priv","r");
    unsigned char* kstr ="XXXXXX";
    if( fp == NULL)
        return NULL;

    RSA *rs = PEM_read_RSAPrivateKey(fp, &rsa, NULL, kstr);

    return rs;
}

int EncryptSimmetricKey(unsigned char *key, unsigned char *sendBuffer, int size, RSA *rsa)
{
    return RSA_public_encrypt(size, key, sendBuffer, rsa, RSA_PKCS1_PADDING );
}

unsigned char* DecryptSimmetricKey(char* ciperKey, RSA* rsa)
{
    unsigned char* simmetrucKey = malloc(RSA_size(rsa)); // RSA_size(rsa) is the modulus
    if( simmetrucKey == NULL)
        return NULL;
    int k = RSA_private_decrypt(RSA_size(rsa), (unsigned char*)ciperKey, simmetrucKey, rsa, RSA_PKCS1_PADDING);
    if( k == -1)
        return NULL;
    return simmetrucKey;
}
