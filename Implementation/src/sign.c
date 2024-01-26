#include <stdlib.h>
#include <strings.h>
#include "../include/parameters.h"
#include "../include/structs.h"
#include "../include/hashing.h"
#include "../include/tree.h"
#include "../include/sample.h"
#include "../include/vectors.h"
#include "../include/lsarray.h"
#include "../lib/randombytes/rng.h"
#include "../lib/XKCP/KeccakHash.h"

void genCommitment(signature* signature, instance instances[TAU], uint8_t pi[N1], uint16_t h[M][N1]) {
    uint8_t random[SECURITY_BYTES * 3] = {0};
    Keccak_HashInstance state;
    uint16_t vector[N1] = {0};
    uint8_t seed[SECURITY_BYTES] = {0};

    #ifdef KAT
    MSEED
    #else
    randombytes(random, SECURITY_BYTES*3);
    memcpy(random, (uint8_t[]){}, sizeof(random));
    #endif
    
    memcpy(seed, random, SECURITY_BYTES);
    memcpy(signature->salt, random + SECURITY_BYTES, SECURITY_BYTES*2);
    keccak_init(&state, 4, signature->salt, seed);

    for(int i = 0; i < TAU; i++) {
        uint8_t **tree = (uint8_t**)instances[i].theta_tree;
        keccak_prg(&state, instances[i].theta_tree[0], SECURITY_BYTES);
        expandTree(signature->salt, (uint8_t(*)[SECURITY_BYTES])tree);
        genInstanceCmts(&instances[i], signature->salt, i, pi);
        genVect(vector, instances[i].pi, instances[i].v, pi);
        genInstanceCmt1(&instances[i], signature->salt, i, h, vector);
    }
}


void genFstChallenge(challenge challenges[TAU], Keccak_HashInstance *state, uint8_t h1[SECURITY_BYTES*2], uint8_t salt[SECURITY_BYTES*2], uint8_t *message, size_t mlen, uint8_t pkbytes[PK_BYTES], instance instances[TAU]) {
    Keccak_HashInstance hash_state;
    uint16_t kappa, nonzero_check;
    int i, j;

    hash_init(&hash_state, salt, NULL, NULL);
    hash_update(&hash_state, message, mlen);
    hash_update(&hash_state, pkbytes, PK_BYTES);
    memcpy(state, &hash_state, sizeof(hash_state));
    for(i = 0; i < TAU; i++) {
        hash_update(&hash_state, (uint8_t*)(instances[i].commitment), SECURITY_BYTES*2);
        hash_update(&hash_state, (uint8_t*)(instances[i].commits), SECURITY_BYTES*2*N2);
    }
    hash_final(&hash_state, h1, 1);

    Keccak_HashInstance prg_stage;
    keccak_init(&prg_stage, 4, NULL, h1);
    for(i = 0; i < TAU; i++) {
        nonzero_check = 0;
        while(!nonzero_check) {
            for(j = 0; j < T; j++) {
                do {
                    keccak_prg(&prg_stage, (uint8_t*)&kappa, 2);
                    kappa = kappa & Q_MASK;
                } while(kappa >= Q);
                challenges[i].kappa[j] = kappa;
                nonzero_check |= kappa;
            }
        }
    }
}


void genFstResponse(instance instances[TAU], challenge challenges[TAU], uint16_t x[T][N1]) {
    int i, j;
    for(i = 0; i < TAU; i++) {
        uint16_t tmp[N1];
        vectorMult(instances[i].s[0], challenges[i].kappa[0], x[0], N1);
        for(j = 1; j < T; j++) {
            vectorMult(tmp, challenges[i].kappa[j], x[j], N1);
            vectorAdd(instances[i].s[0], instances[i].s[0], tmp, N1);
        }
        for(j = 0; j < N2; j++) {
            vectorPermute(tmp, instances[i].pi[j], instances[i].s[j]);
            vectorAdd(instances[i].s[j+1], tmp, instances[i].v[j], N1);
        }
    }
}


void genScndChallenge(challenge challenges[TAU], Keccak_HashInstance* state, uint8_t h1[SECURITY_BYTES*2], uint8_t h2[SECURITY_BYTES*2], instance instances[TAU]) {
    Keccak_HashInstance prg_state;
    uint16_t alpha;
    int i;

    hash_update(state, h1, SECURITY_BYTES*2);
    for(i = 0; i < TAU; i++)
        hash_update(state, (uint8_t*)instances[i].s[1], N1*N2*2);
    hash_final(state, h2, 2);
    
    keccak_init(&prg_state, 4, NULL, h2);
    for(i = 0; i < TAU; i++) {
        keccak_prg(&prg_state, (uint8_t*)&alpha, 2);
        alpha = (alpha & N_MASK) + 1;
        challenges[i].alpha = alpha;
    }
}


void genScndResponse(instance instances[TAU], challenge challenges[TAU], signature* signature) {
    for(int i = 0; i < TAU; i++) {
        memcpy(signature->responses[i].z, instances[i].s[challenges[i].alpha], 2*N1);
        if(challenges[i].alpha != 1)
            memcpy(signature->responses[i].pi, instances[i].pi[0], N1);
        else
            for(int j = 0; j < N1; j++)
                signature->responses[i].pi[j] = j;
        getPartialTheta(signature->responses[i].theta, instances[i].theta_tree, challenges[i].alpha - 1);
        memcpy(signature->responses[i].commitment, instances[i].commits[challenges[i].alpha - 1], SECURITY_BYTES*2);
    }
}


void signatureToBytes(uint8_t signedbytes[SIGNATURE_BYTES], signature* signature) {
    memcpy(signedbytes, signature->salt, SECURITY_BYTES*2);
    signedbytes += SECURITY_BYTES*2;
    memcpy(signedbytes, signature->h1, SECURITY_BYTES*2);
    signedbytes += SECURITY_BYTES*2;
    memcpy(signedbytes, signature->h2, SECURITY_BYTES*2);
    signedbytes += SECURITY_BYTES*2;

    int i;
    for(i = 0; i < TAU; i++) {
        memcpy(signedbytes, signature->responses[i].commitment, SECURITY_BYTES*2);
        signedbytes += SECURITY_BYTES*2;
        memcpy(signedbytes, signature->responses[i].theta, SECURITY_BYTES * THETA_TREE);
        signedbytes += SECURITY_BYTES * THETA_TREE;
    }

    for(i = 0; i < TAU * N1; i++)
        s10bitInArray(signedbytes, i, signature->responses[i/N1].z[i%N1]);
    signedbytes += (TAU*N1*10 + 7)/8;

    for(i = 0; i < ((TAU*N1)/2); i++)
        s2coeffFromArray(signedbytes, i, signature->responses[(i*2)/N1].pi[(i*2)%N1], signature->responses[(i*2+1)/N1].pi[(i*2+1)%N1]);
}


void sign(unsigned char signedmessage[32+SIGNATURE_BYTES], size_t* smlen, unsigned char message[32], size_t mlen, secret_key* sk, public_key* pk) {
    for(int i = 0; i < mlen; i++)
        signedmessage[SIGNATURE_BYTES + i] = message[i];

    signature* signature = malloc(sizeof(struct signature));
    instance* instances = malloc(sizeof(instance)*TAU);
    challenge challenges[TAU] = {0};
    Keccak_HashInstance state;

    genCommitment(signature, instances, sk->pi, pk->h);
    genFstChallenge(challenges, &state, signature->h1, signature->salt, signedmessage+SIGNATURE_BYTES, mlen, sk->pkbytes, instances);
    genFstResponse(instances, challenges, pk->x);
    genScndChallenge(challenges, &state, signature->h1, signature->h2, instances);
    genScndResponse(instances, challenges, signature);

    #ifdef KAT
    printf("theta\n");
    for(int i = 0; i < TAU; i++) {
        for(int j = 0; j < SECURITY_BYTES; j++)
            printf("%2x", instances[i].theta_tree[0][j]);
        printf("}\n");
    }

    printf("commitment\n");
    for(int i = 0; i < TAU; i++) {
        for(int j = 0; j < 2*SECURITY_BYTES; j++)
            printf("%2x", instances[i].commitment[j]);
        printf("}\n");
    }

    printf("v\n");
    for(int i = 0; i < TAU; i++) {
        for(int j = 0; j < N1; j++)
            printf("%d ", instances[i].v[0][j]);
        printf("}\n");
    }

    printf("kappas\n");
    for(int i = 0; i < TAU; i++) {
        for(int j = 0; j < T; j++) 
            printf("%d ", challenges[i].kappa[j]);
        printf("}\n");
    }

    printf("alphas\n");
    for(int i = 0; i < TAU; i++)
        printf("%d ", challenges[i].alpha);
    printf("\n");
    
    printf("h1\n");
    for(int i = 0; i < SECURITY_BYTES*2; i++)
        printf("%x", signature->h1[i]);
    printf("\n");

    printf("h2\n");
    for(int i = 0; i < SECURITY_BYTES*2; i++)
        printf("%x", signature->h2[i]);
    printf("\n");
    #endif

    signatureToBytes(signedmessage, signature);
    free(signature);
    free(instances);
}
