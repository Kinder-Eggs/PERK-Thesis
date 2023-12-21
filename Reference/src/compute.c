#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "../include/parameters.h"
#include "../include/structs.h"
#include "../include/lsarray.h"
#include "../include/hashing.h"
#include "../lib/djbsort/djbsort.h"
#include "../lib/XKCP/KeccakHash.h"


#define Z_USED_BITS (uint8_t)((TAU * N1 * 10 + 7) % 8 + 1)
static const uint8_t z_unused_mask = (uint8_t)(((1U << (Z_USED_BITS)) - 1) ^ 0xFFU);
#define PI_USED_BITS (uint8_t)((TAU * N1 * N1_BITS_TWICE / 2 + 7) % 8 + 1)
static const uint8_t pi_unused_mask = (uint8_t)(((1U << (PI_USED_BITS)) - 1) ^ 0xFFU);


uint16_t barret_reduce32(uint32_t value) {
    uint32_t t;
    const uint64_t v =(((uint64_t)1U << 41) / Q);
    t = ((v * value + v) >> 41);
    t *= Q;
    return (uint16_t)(value - t);    
}


uint16_t multInverse(uint16_t input) {
    uint16_t exponent, result = 1;

    for(exponent = Q-2; exponent > 0; exponent >>= 1) {
        if((exponent & 0x1) == 1)
            result = barret_reduce32((uint32_t)result * (uint32_t)input);
        input = barret_reduce32((uint32_t)input * (uint32_t)input);
    }
    return result;
}


void compute_y(uint16_t y[T][M], uint8_t pi[N1], uint16_t x[T][N1], uint16_t h[M][N1]) {
    uint32_t temp[N1];
    uint16_t permuted[N1];
    uint32_t partsum;
    int i, j, k;
    for(i = 0; i < T; i++) {
        for(j = 0; j < N1; j++)
            temp[j] = ((uint32_t)pi[j] << 16) | x[i][j];
        uint32_sort(temp, N1);
        for(j = 0; j < N1; j++)
            permuted[j] = (uint16_t)temp[j];
    
        for(j = 0; j < M; j++) {
            partsum = 0;
            for(k = 0; k < N1; k++)
                partsum += (uint32_t)h[j][k] * (uint32_t)permuted[k];
            y[i][j] = barret_reduce32(partsum);
        }
    }
}


int compute_rank(uint16_t input[T][N1]) {
    int found, i, tmp, row, col, rank = 0;
    uint16_t mult;
    
    uint16_t x[T][N1];
    memcpy(x, input, N1*T*2);
    for(col = 0; col < N1; col++) {
        found = 0;
        for(i = rank; i < T; i++) {
            if(x[i][col] != 0) {
                row = i;
                found = 1;
                break;
            }
        }
        if(found) {
            for(i = 0; i < N1; i++) {
                tmp = x[rank][i];
                x[rank][i] = x[row][i];
                x[row][i] = tmp;
            }
            for(row = rank + 1; row < T; row++) {
                mult = barret_reduce32((uint32_t)x[row][col] 
                * (uint32_t)multInverse(x[rank][col]));
                for(i = col; i < N1; i++) {
                    x[row][i] -= barret_reduce32((uint32_t)mult
                    * (uint32_t)x[rank][i]);
                }
            }
            rank++;
        }
        if(rank == T)
            break;
    }
    return rank;
}


void computeInverse(uint8_t o[N1], const uint8_t p[N1]) {
    uint32_t buffer[N1];
    int i;
    for(i = 0; i < N1; i++) 
        buffer[i] = (((uint32_t)p[i]) << 16) | i;
    uint32_sort(buffer, N1);
    for(i = 0; i < N1; i++)
        o[i] = (uint16_t)(buffer[i]);
}


void computeComposeInv(uint8_t o[N1], const uint8_t p1[N1], const uint8_t p2[N1]) {
    uint32_t buffer[N1];
    int i;
    for(i = 0; i < N1; i++) 
        buffer[i] = (((uint32_t)p2[i]) << 16) | p1[i];
    uint32_sort(buffer, N1);
    for(i = 0; i < N1; i++)
        o[i] = (uint16_t)(buffer[i]);
}


void computeCompose(uint8_t o[N1], const uint8_t p1[N1], const uint8_t p2[N1]) {
    uint32_t buffer[N1];
    uint8_t tmp[N1];
    computeInverse(tmp, p2);
    int i;
    for(i = 0; i < N1; i++) 
        buffer[i] = (((uint32_t)tmp[i]) << 16) | p1[i];
    uint32_sort(buffer, N1);
    for(i = 0; i < N1; i++)
        o[i] = (uint16_t)(buffer[i]);
}


void convToBytes(uint8_t pkbytes[PK_BYTES], public_key *pk) {
    memcpy(pkbytes, pk->seed, SECURITY_BYTES);
    for(int i = 0; i < M*T; i++)
        storeInArray(pkbytes + SECURITY_BYTES, i, pk->y[i/M][i%M]);
}


void mulMatrixVect(uint16_t out[M], uint16_t h[M][N1], uint16_t vector[N1]) {
    uint32_t tmp;
    for(int i = 0; i < M; i++) {
        tmp = 0;
        for(int j = 0; j < N1; j++)
            tmp += ((uint32_t)h[i][j]) * ((uint32_t)vector[j]);
        out[i] = barret_reduce32(tmp);
    }
}


uint8_t checkPermutation(const uint8_t p[N1]) {
    uint8_t c[N1 / sizeof(**((uint8_t**)0))] = {0};
    //uint8_t c[N1] = {0};

    for (uint32_t i = 0; i < sizeof(c); i++) {
        if (p[i] >= sizeof(c))
            return 0;
        if (c[p[i]])
            return 0;
        c[p[i]] = 1;
    }
    return 1;
}


uint8_t getSignature(signature* signature, unsigned char signedmessage[SIGNATURE_BYTES]) {
    int i;
    uint16_t z, pi1, pi2;
    
    memcpy(signature->salt, signedmessage, SECURITY_BYTES*2);
    signedmessage += SECURITY_BYTES*2;
    memcpy(signature->h1, signedmessage, SECURITY_BYTES*2);
    signedmessage += SECURITY_BYTES*2;
    memcpy(signature->h2, signedmessage, SECURITY_BYTES*2);
    signedmessage += SECURITY_BYTES*2;

    for(i = 0; i < TAU; i++) {
        memcpy(signature->responses[i].commitment, signedmessage, SECURITY_BYTES*2);
        signedmessage += SECURITY_BYTES*2;
        memcpy(signature->responses[i].theta, signedmessage, SECURITY_BYTES*THETA_TREE);
        signedmessage += SECURITY_BYTES*THETA_TREE;
    }

    for(i = 0; i < TAU * N1; i++) {
        z = l10bitFromArray(signedmessage, i);
        if(z >= Q) {
            return 0;
        }
        signature->responses[i/N1].z[i%N1] = z;
    }
    signedmessage += ((TAU*N1*10+7)/8)-1;

    if(signedmessage[0] & z_unused_mask)
        return 0;

    signedmessage += 1;
    for(i = 0; i < ((TAU*N1)/2); i++) {
        l2coeffFromArray(&pi1, &pi2, signedmessage, i);
        signature->responses[(i*2)/N1].pi[(i*2)%N1] = pi1;
        signature->responses[(i*2+1)/N1].pi[(i*2+1)%N1] = pi2;
    }
    signedmessage += ((TAU*N1*N1_BITS_TWICE/2+7)/8)-1;

    if(signedmessage[0] & pi_unused_mask)
        return 0;

    for(int i = 0; i < TAU; i++) {
        if(!checkPermutation(signature->responses[i].pi))
            return 0;
    }

    return 1;
}


void getChallenges(challenge challenges[TAU], uint8_t h1[SECURITY_BYTES*2], uint8_t h2[SECURITY_BYTES*2]) {
    Keccak_HashInstance h1state, h2state;
    uint16_t kappa, nonzero_check, alpha;
    int i, j;

    keccak_init(&h1state, 4, NULL, h1);
    for(i = 0; i < TAU; i++) {
        nonzero_check = 0;
        while(!nonzero_check) {
            for(j = 0; j < T; j++) {
                do {
                    keccak_prg(&h1state, (uint8_t*)&kappa, 2);
                    kappa = kappa & Q_MASK;
                } while(kappa >= Q);
                challenges[i].kappa[j] = kappa;
                nonzero_check |= kappa;
            }
        }
    }
    
    keccak_init(&h2state, 4, NULL, h2);
    for(i = 0; i < TAU; i++) {
        keccak_prg(&h2state, (uint8_t*)&alpha, 2);
        alpha = (alpha & N_MASK) + 1;
        challenges[i].alpha = alpha;
    }
}


void computeh1(uint8_t h1[SECURITY_BYTES*2], Keccak_HashInstance* saved_state, uint8_t salt[SECURITY_BYTES*2], unsigned char* message, size_t mlen, uint8_t* pkbytes, instance instances[TAU]) {
    Keccak_HashInstance state;

    hash_init(&state, salt, NULL, NULL);
    hash_update(&state, message, mlen);
    hash_update(&state, pkbytes, PK_BYTES);
    memcpy(saved_state, &state, sizeof(state));
    for(int i = 0; i < TAU; i++) {
        hash_update(&state, (uint8_t*)instances[i].commitment, SECURITY_BYTES*2);
        hash_update(&state, (uint8_t*)instances[i].commits, SECURITY_BYTES*2*N2);
    }
    hash_final(&state, h1, 1);
}


void computeh2(uint8_t h2[SECURITY_BYTES*2], Keccak_HashInstance* saved_state, uint8_t h1[SECURITY_BYTES*2], instance instances[TAU]) {
    hash_update(saved_state, h1, SECURITY_BYTES*2);
    for(int i = 0; i < TAU; i++)
        hash_update(saved_state, (uint8_t*)instances[i].s[1], N1*2*N2);
    hash_final(saved_state, h2, 2);
}

