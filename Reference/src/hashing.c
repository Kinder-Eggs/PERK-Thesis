#include <stdint.h>
#include "../include/parameters.h"
#include "../include/structs.h"
#include "../lib/XKCP/KeccakHash.h"

void keccak_init(Keccak_HashInstance *state, uint8_t domain, uint8_t salt[2*SK_SIZE], uint8_t seed[LAMBDA/8]) {
    Keccak_HashInitialize_SHAKE(state);
    if(salt != NULL)
        Keccak_HashUpdate(state, salt, 2*SK_SIZE*8);
    if(seed != NULL)
        Keccak_HashUpdate(state, seed, LAMBDA);
    Keccak_HashUpdate(state, &domain, 8);
    Keccak_HashFinal(state, NULL);
}


void keccak_prg(Keccak_HashInstance *state, uint8_t *output, size_t outlen) {
    Keccak_HashSqueeze(state, output, 8*outlen);
}


void hash_init(Keccak_HashInstance *state, uint8_t salt[2*SECURITY_BYTES], uint8_t *tau, uint8_t *n) {
    Keccak_HashInitialize_SHA3(state);
    Keccak_HashUpdate(state, salt, SECURITY_BYTES*2*8);

    uint8_t counters[2];
    int j = 0;
    if(tau != NULL) {
        counters[j] = *tau;
        j++;
    }
    if(n != NULL) {
        counters[j] = *n;
        j++;
    }
    if (j != 0)
        Keccak_HashUpdate(state, counters, j*8);
}


void hash_update(Keccak_HashInstance *state, uint8_t *message, size_t size) {
    Keccak_HashUpdate(state, message, size*8);
}


void hash_final(Keccak_HashInstance *state, uint8_t digest[2*SECURITY_BYTES], uint8_t domain) {
    Keccak_HashUpdate(state, &domain, 8);
    Keccak_HashFinal(state, digest);
}