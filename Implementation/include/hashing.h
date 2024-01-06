#ifndef HASHING
#define HASHING

#include <stdint.h>
#include <stdlib.h>
#include "parameters.h"
#include "structs.h"
#include "../lib/XKCP/KeccakHash.h"


void keccak_init(Keccak_HashInstance *state, uint8_t domain, uint8_t salt[2*SK_SIZE], uint8_t seed[LAMBDA/8]);
void keccak_prg(Keccak_HashInstance *state, uint8_t *output, size_t outlen);

void hash_init(Keccak_HashInstance *state, uint8_t salt[SECURITY_BYTES*2], uint8_t *tau, uint8_t *n);
void hash_update(Keccak_HashInstance *state, uint8_t *message, size_t size);
void hash_final(Keccak_HashInstance *state, uint8_t digest[2*SECURITY_BYTES], uint8_t domain);

#endif