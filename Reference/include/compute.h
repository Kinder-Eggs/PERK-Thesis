#ifndef COMPUTE
#define COMPUTE

#include <stdint.h>
#include "structs.h"
#include "parameters.h"
#include "../lib/XKCP/KeccakHash.h"

uint16_t barret_reduce32(uint32_t value);
int compute_rank(uint16_t input[T][N1]);
void compute_y(uint16_t y[T][M], uint8_t pi[N1], uint16_t x[T][N1], uint16_t h[M][N1]);
void computeInverse(uint8_t o[N1], uint8_t p[N1]);
void computeComposeInv(uint8_t o[N1], uint8_t p1[N1], uint8_t p2[N1]);
void computeCompose(uint8_t o[N1], uint8_t p1[N1], uint8_t p2[N1]);
void convToBytes(uint8_t pkbytes[PK_BYTES], public_key *pk);
void mulMatrixVect(uint16_t out[M], uint16_t h[M][N1], uint16_t vector[N1]);
uint8_t getSignature(signature* signature, unsigned char signedmessage[32+SIGNATURE_BYTES]);
void getChallenges(challenge challenges[TAU], uint8_t h1[SECURITY_BYTES*2], uint8_t h2[SECURITY_BYTES*2]);
void computeh1(uint8_t h1[SECURITY_BYTES*2], Keccak_HashInstance* saved_state, uint8_t salt[SECURITY_BYTES*2], unsigned char* message, size_t mlen, uint8_t* pkbytes, instance instances[TAU]);
void computeh2(uint8_t h2[SECURITY_BYTES*2], Keccak_HashInstance* saved_state, uint8_t h1[SECURITY_BYTES*2], instance instances[TAU]);


#endif
