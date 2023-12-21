#include <stdint.h>
#include "../include/parameters.h"
#include "../include/compute.h"
#include "../include/hashing.h"
#include "../lib/XKCP/KeccakHash.h"
#include "../lib/djbsort/djbsort.h"


int permute(uint8_t pi[N1], uint16_t buffer[N1]) {
    uint32_t buffer2[N1];
    int i;
    for(i = 0; i < N1; i++)
        buffer2[i] = (((uint32_t)buffer[i]) << 16) | i;

    uint32_sort(buffer2, N1);
    for(i = 1; i < N1; i++) {
        if((buffer2[i-1] >> 16) == (buffer2[i] >> 16)) {
           return 0;
        }
    }

    for(i = 0; i < N1; i++)
        pi[i] = (uint16_t)buffer2[i];
    return 1;
}


void sample_pi(uint8_t pi[N1], uint8_t seed[SECURITY_BYTES]) {
    uint16_t buffer[N1];
    Keccak_HashInstance state;
    keccak_init(&state, 4, NULL, seed);
    keccak_prg(&state, (uint8_t*) buffer, N1*2);

    while(permute(pi, buffer) == 0)
        keccak_prg(&state, (uint8_t *)buffer, N1*2);
    memset(buffer, 0, 2*N1);
}


void sample_h(uint16_t h[M][N1], uint8_t seed[LAMBDA/8]) {
    Keccak_HashInstance state;
    uint16_t buffer[SHAKE_BLOCK_SIZE/2];
    keccak_init(&state, 4, NULL, seed);
    int j = 0, k;
    while(j < M*N1) {
        keccak_prg(&state, (uint8_t *)buffer, SHAKE_BLOCK_SIZE);
        //Might need to change for while, check arith line 141
        for(k = 0; (j < M*N1) && (k < SHAKE_BLOCK_SIZE/2); k++) {
            h[j/N1][j%N1] = Q_MASK & buffer[k];
            if(h[j/N1][j%N1] < Q)
                j++;
        }
    }
}


void sample_x(uint16_t x[T][N1], uint8_t seed[LAMBDA/8]) {
    Keccak_HashInstance state;
    uint16_t buffer[SHAKE_BLOCK_SIZE/2];
    keccak_init(&state, 4, NULL, seed);
    int i = 0, rank = 0, j;
    while(rank != T) {
        while(i < N1*T) {
            keccak_prg(&state, (uint8_t *)buffer, SHAKE_BLOCK_SIZE);
            //Might need to change for while, check arith line 100
            for(j = 0; (i < N1*T) && (j < SHAKE_BLOCK_SIZE/2); j++) {
                x[i/N1][i%N1] = Q_MASK & buffer[j];
                if(x[i/N1][i%N1] < Q)
                    i++;
            }
        }
        rank = compute_rank(x);
    }
}


void genPiV(instance* instance, uint8_t salt[SECURITY_BYTES*2]) {
    Keccak_HashInstance statepiv;
    uint16_t random_pi[N1];
    uint16_t random_v[SHAKE_BLOCK_SIZE/2];
    int i, j, k;
    for(i = 0; i < N2; i++) {
        keccak_init(&statepiv, 4, salt, instance->theta_tree[N2-1+i]);
        if(i != 0) {
            keccak_prg(&statepiv, (uint8_t*)random_pi, N1*2);
            while(permute(instance->pi[i], random_pi) != 1)
                keccak_prg(&statepiv, (uint8_t*)random_pi, N1*2);
        }

        keccak_init(&statepiv, 5, salt, instance->theta_tree[N2-1+i]);
        j = 0;
        while(j < N1) {
            keccak_prg(&statepiv, (uint8_t*)random_v, SHAKE_BLOCK_SIZE);
            for(k = 0; (j < N1) && (k < SHAKE_BLOCK_SIZE/2); k++) {
                instance->v[i][j] = Q_MASK & random_v[k];
                if(instance->v[i][j] < Q)
                    j++;
            }
        }
    }
}


void genCmts(instance* instance, uint8_t salt[SECURITY_BYTES*2], uint8_t tau) {
    uint8_t idx;
    uint8_t pi_bytes[N1];
    int i, j;
    for(i = 0; i < N2; i++) {
        Keccak_HashInstance statecmt;
        idx = i;
        if(i == 0) {
            for(j = 0; j < N1; j++)
                pi_bytes[j] = (uint8_t)instance->pi[0][j];
            hash_init(&statecmt, salt, &tau, &idx);
            hash_update(&statecmt, pi_bytes, N1);
            hash_update(&statecmt, instance->theta_tree[N2-1], SECURITY_BYTES);
            hash_final(&statecmt, instance->commits[0], 0);
        } else {
            hash_init(&statecmt, salt, &tau, &idx);
            hash_update(&statecmt, instance->theta_tree[N2-1+i], SECURITY_BYTES);
            hash_final(&statecmt, instance->commits[i], 0);
        }
    }
}


void genInstanceCmts(instance *instance, uint8_t salt[SECURITY_BYTES*2], uint8_t tau, uint8_t pi[N1]) {
    genPiV(instance, salt);
    //Instance pi 1 generation
    computeInverse(instance->pi[0], instance->pi[1]);
    for(int i = 2; i < N2; i++)
        computeComposeInv(instance->pi[0], instance->pi[0], instance->pi[i]);
    computeCompose(instance->pi[0], instance->pi[0], pi);

    genCmts(instance, salt, tau);
}


void genInstanceCmt1(instance *instance, uint8_t salt[SECURITY_BYTES*2], int tau, uint16_t h[M][N1], uint16_t vector[N1]) {
    Keccak_HashInstance state;
    uint16_t tmp[M];
    mulMatrixVect(tmp, h, vector);
    hash_init(&state, salt, (uint8_t*)&tau, NULL);
    hash_update(&state, (uint8_t*)tmp, M*2);
    hash_final(&state, instance->commitment, 0);
}