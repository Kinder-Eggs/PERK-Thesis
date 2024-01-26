#include "../include/structs.h"
#include "../lib/randombytes/rng.h"
#include "../include/sample.h"
#include "../include/compute.h"
#include <string.h>
#include <stdio.h>

void keyGen(secret_key *sk, public_key *pk) {
    #ifdef KAT
    SKSEED
    PKSEED
    #else
    randombytes(sk->seed, LAMBDA/8);
    randombytes(pk->seed, LAMBDA/8);
    #endif

    sample_pi(sk->pi, sk->seed);

    #ifdef KAT
    printf("Pi Sample\n");
    for(int i = 0; i < N1; i++)
        printf("%x ", (sk->pi)[i]);
    printf("\n");
    #endif

    sample_h(pk->h, pk->seed);

    #ifdef KAT
    printf("h Sample\n");
    for(int i = 0; i < M; i++) {
        for(int j = 0; j < 5; j++)
            printf("%d ", (pk->h)[i][j]);
        printf("}\n");
    }
    printf("\n");
    #endif 

    sample_x(pk->x, pk->seed);

    #ifdef KAT
    printf("x Sample\n");
    for(int i = 0; i < T; i++) {
        for(int j = 0; j < 5; j++)
            printf("%d ", (pk->x)[i][j]);
        printf("}\n");
    }
    #endif

    compute_y(pk->y, sk->pi, pk->x, pk->h);
    
    #ifdef KAT
    printf("y Computation\n");
    for(int i = 0; i < T; i++) {
        for(int j = 0; j < 5; j++)
            printf("%d ", (pk->y)[i][j]);
        printf("}\n");
    }
    #endif

    convToBytes(sk->pkbytes, pk);
}
