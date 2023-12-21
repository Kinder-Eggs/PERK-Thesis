#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/structs.h"
#include "../include/compute.h"
#include "../include/hashing.h"
#include "../include/vectors.h"
#include "../include/sample.h"
#include "../include/tree.h"
#include "../lib/XKCP/KeccakHash.h"


void computeSs(uint16_t s[N2+1][N1], uint8_t pi[N2][N1], uint16_t v[N2][N1], uint16_t alpha) {
    uint16_t temp[N1];
    int i, j;
    for(i = 0; i < N2; i++) {
        if(i+1 != alpha) {
            for(j = 0; j < N1; j++)
                temp[pi[i][j]] = s[i][j];
            vectorAdd(s[i+1], temp, v[i], N1);
        }
    }
}


void verGenInstanceCmts(instance* instance, response* response, challenge challenge, uint16_t x[T][N1], uint8_t tau, uint8_t salt[SECURITY_BYTES*2]) {
    uint16_t tmp[N1];
    int i, j;
    expandPartialTree(salt, instance->theta_tree, response->theta, challenge.alpha-1);

    genPiV(instance, salt);
    if(challenge.alpha != 1)
        memcpy(instance->pi[0], response->pi, N1);

    vectorMult(instance->s[0], challenge.kappa[0], x[0], N1);
    for(i = 1; i < T; i++) {
        vectorMult(tmp, challenge.kappa[i], x[i], N1);
        vectorAdd(instance->s[0], instance->s[0], tmp, N1);
    }

    memcpy(instance->s[challenge.alpha], (uint8_t*)response->z, N1*2);
    computeSs(instance->s, instance->pi, instance->v, challenge.alpha);
    genCmts(instance, salt, tau);

    if(challenge.alpha != 1) {
        uint8_t idx = 0, pibytes[N1];
        Keccak_HashInstance state;
        for(j = 0; j < N1; j++)
            pibytes[j] = instance->pi[0][j];
        hash_init(&state, salt, &tau, &idx);
        hash_update(&state, pibytes, N1);
        hash_update(&state, instance->theta_tree[N2-1], SECURITY_BYTES);
        hash_final(&state, instance->commits[0], 0);
    }
    memcpy(instance->commits[challenge.alpha-1], (uint8_t*)response->commitment, SECURITY_BYTES*2);
}


void verGenInstanceCmt1(instance* instance, uint16_t kappa[T], public_key* pk, uint8_t tau, uint8_t salt[SECURITY_BYTES*2]) {
    Keccak_HashInstance state;
    uint16_t temp1[M], temp2[M], temp3[M];

    mulMatrixVect(temp1, pk->h, instance->s[N2]);
    vectorMult(temp2, kappa[0], pk->y[0], M);
    for(uint8_t i = 1; i < T; i++) {
        vectorMult(temp3, kappa[i], pk->y[i], M);
        vectorAdd(temp2, temp2, temp3, M);
    }
    vectorSub(temp1, temp1, temp2, M);

    hash_init(&state, salt, &tau, NULL);
    hash_update(&state, (uint8_t*) temp1, M*2);
    hash_final(&state, instance->commitment, 0);
}


uint8_t verify(unsigned char message[32], size_t* mlen, unsigned char signedmessage[32+SIGNATURE_BYTES], size_t smlen, uint8_t pkbytes[PK_BYTES], public_key* pk) {
    signature signature;
    //challenge challenges[TAU];
    challenge* challenges = malloc(sizeof(challenge)*TAU);
    *mlen = -1;

    if(!getSignature(&signature, signedmessage))
        return 0;
    getChallenges(challenges, signature.h1, signature.h2);

    //Verify
    //instance instances[TAU];
    uint8_t h1prime[SECURITY_BYTES*2], h2prime[SECURITY_BYTES*2];
    Keccak_HashInstance state;
    uint8_t i, j;
    for(i = 0; i < TAU; i++)
        if(challenges[i].alpha == 1)
            for(j = 0; j < N1; j++)
                if(signature.responses[i].pi[j] != j)
                    return 0;
                
    instance* instances = malloc(sizeof(instance)*TAU);

    for(i = 0; i < TAU; i++) {
        verGenInstanceCmts(&instances[i], &signature.responses[i], challenges[i], pk->x, i, signature.salt);
        verGenInstanceCmt1(&instances[i], challenges[i].kappa, pk, i, signature.salt);
    }
    
    computeh1(h1prime, &state, signature.salt, signedmessage+SIGNATURE_BYTES, 32, pkbytes, instances);
    computeh2(h2prime, &state, h1prime, instances);

    #ifdef KAT
    printf("h1prime\n");
    for(int i = 0; i < SECURITY_BYTES*2; i++)
        printf("%x", h1prime[i]);
    printf("\n");

    printf("h2prime\n");
    for(int i = 0; i < SECURITY_BYTES*2; i++)
        printf("%x", h2prime[i]);
    printf("\n");
    #endif
    
    free(challenges);
    free(instances);
    
    if(memcmp((uint8_t*)h1prime, (uint8_t*)signature.h1, SECURITY_BYTES*2))
        return 0;
    if(memcmp((uint8_t*)h2prime, (uint8_t*)signature.h2, SECURITY_BYTES*2))
        return 0;

    *mlen = 32;
    memcpy(message, signedmessage+SIGNATURE_BYTES, 32);

    return 1;
}
