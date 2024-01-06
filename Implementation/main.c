#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "./include/api.h"
#include "./include/parameters.h"
#include "./include/structs.h"
#include "./lib/randombytes/rng.h"


int main() {
    uint8_t *seed = malloc(sizeof(uint8_t)*48);
    randombytes_init(seed, NULL, 256);
    free(seed);

    secret_key* sk = (secret_key*)malloc(sizeof(secret_key));
    public_key* pk = (public_key*)malloc(sizeof(public_key));

    keyGen(sk, pk);
    unsigned char message[32] = {0};
    size_t mlen = 32;
    unsigned char signedmessage[32 + SIGNATURE_BYTES];
    size_t smlen = 32 + SIGNATURE_BYTES;

    sign(signedmessage, &smlen, message, mlen, sk, pk);

    if(verify(message, &mlen, signedmessage, smlen, sk->pkbytes, pk))
        printf("Signature verified\n");
    else
        printf("Signature denied\n");

    free(sk);
    free(pk);
}