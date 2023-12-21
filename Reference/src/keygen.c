#include "../include/structs.h"
#include "../lib/randombytes/rng.h"
#include "../include/sample.h"
#include "../include/compute.h"
#include <string.h>

#include <stdio.h>

void keyGen(secret_key *sk, public_key *pk) {
    #ifdef KAT
    memcpy(sk->seed, (uint8_t[]){0x79, 0x67, 0x32, 0xac, 0xba, 0x3e, 0xfd, 0xf7, 0x31, 0xbf, 0x7c, 0x24, 0x2a, 0xee, 0xdd, 0xf5, 0xeb, 0xa5, 0xb1, 0x31, 0xda, 0x90, 0xe3, 0x6a, 0xf2, 0x3a, 0x3b, 0xce, 0x9c, 0x7a, 0xa9, 0x3a}, sizeof(sk->seed));
    memcpy(pk->seed, (uint8_t[]){0x91, 0x61, 0x8f, 0xe9, 0x9a, 0x8f, 0x94, 0x20, 0x49, 0x7b, 0x24, 0x6f, 0x73, 0x5b, 0x27, 0xa0, 0x19, 0x07, 0x8a, 0x9d, 0x3c, 0xa6, 0xb2, 0xa0, 0x01, 0xae, 0xc0, 0xb9, 0xe0, 0x7e, 0x68, 0x0b}, sizeof(pk->seed));
    //memcpy(sk->seed, (uint8_t[]){}, sizeof(sk->seed));
    //memcpy(pk->seed, (uint8_t[]){}, sizeof(pk->seed));
    #else
    randombytes(sk->seed, LAMBDA/8);
    randombytes(pk->seed, LAMBDA/8);
    #endif

    sample_pi(sk->pi, sk->seed);
    sample_h(pk->h, pk->seed);
    sample_x(pk->x, pk->seed);
    compute_y(pk->y, sk->pi, pk->x, pk->h);

    convToBytes(sk->pkbytes, pk);

    #ifdef KAT
    printf("Pi Sample\n");
    for(int i = 0; i < N1; i++)
        printf("%x ", (sk->pi)[i]);
    printf("\n");

    printf("h Sample\n");
    for(int i = 0; i < M; i++) {
        for(int j = N1-5; j < N1; j++)
            printf("%d ", (pk->h)[i][j]);
        printf("}\n");
    }
    printf("\n");

    printf("x Sample\n");
    for(int i = 0; i < T; i++) {
        for(int j = 0; j < 5; j++)
            printf("%d ", (pk->x)[i][j]);
        printf("}\n");
    }

    printf("y Computation\n");
    for(int i = 0; i < T; i++) {
        for(int j = 0; j < 5; j++)
            printf("%d ", (pk->y)[i][j]);
        printf("}\n");
    }
    #endif
}
