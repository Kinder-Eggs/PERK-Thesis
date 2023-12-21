#ifndef STRUCTS
#define STRUCTS

#include <stdint.h>
#include "parameters.h"

typedef struct secret_key{
    uint8_t seed[LAMBDA/8];
    uint8_t pi[N1];
    uint8_t pkbytes[PK_BYTES];
} secret_key;

typedef struct public_key{
    uint8_t seed[LAMBDA/8];
    uint16_t h[M][N1];
    uint16_t x[T][N1];
    uint16_t y[T][M];
} public_key;

typedef struct instance{
    uint8_t theta_tree[2*N2-1][SECURITY_BYTES];
    uint8_t pi[N2][N1];
    uint16_t v[N2][N1];
    uint16_t s[N2+1][N1];
    uint8_t commitment[2*SECURITY_BYTES];
    uint8_t commits[N2][2*SECURITY_BYTES];
} instance;

typedef struct challenge{
    uint16_t kappa[T];
    uint16_t alpha;
} challenge;

typedef struct response{
    uint16_t z[N1];
    uint8_t pi[N1];
    uint8_t theta[THETA_TREE][SECURITY_BYTES];
    uint8_t commitment[2*SECURITY_BYTES];
} response;

typedef struct signature{
    uint8_t salt[SECURITY_BYTES*2];
    uint8_t h1[SECURITY_BYTES*2];
    uint8_t h2[SECURITY_BYTES*2];
    response responses[TAU];
} signature;

#endif