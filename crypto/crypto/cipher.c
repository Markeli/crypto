#include <openssl/evp.h>
#include <common.h>
#include <string.h>
#include <cipher.h>
#include <openssl/rand.h>

void Generate_AES_256_KEY(char key[32])
{
    if (RAND_bytes(key, 32) == 0)
    {
        perror("AES keu genretion");
        exit(1);
    }
}


int DoCrypt()
{
    int inLength, outLength,i;
    unsigned char key[32];
    unsigned char iv[8];
    EVP_CIPHER_CTX ctx;
    const EVP_CIPHER * cipher;

    EVP_CIPHER_CTX_init(&ctx);

    cipher = EVP_aes_256_cfb();

    EVP_EncryptInit(&ctx, cipher, key, iv);
    for (i=0; i<32; ++i)
    {
        printf("%d", key[i]);
    }
    printf("\n");
    EVP_CIPHER_CTX_cleanup(&ctx);
    return 1;
}
