#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "./include/api.h"
#include "./include/parameters.h"
#include "./include/structs.h"
#include "./lib/randombytes/rng.h"
#include "./m1cycles.h"


int main() {
    uint8_t seed[48] = {0};
    randombytes_init(seed, NULL, 256);
    SETUP_COUNTER();
    secret_key* sk = (secret_key*)malloc(sizeof(secret_key));
    public_key* pk = (public_key*)malloc(sizeof(public_key));

    WRAP_FUNC("keygen: " CYCLE_TYPE "\n",
        cycles, time0, time1,
        keyGen(sk, pk));

    unsigned char message[32] = {0};
    size_t mlen = 32;
    unsigned char signedmessage[32 + SIGNATURE_BYTES];
    size_t smlen = 32 + SIGNATURE_BYTES;
    
    WRAP_FUNC("sign: " CYCLE_TYPE "\n",
        cycles, time0, time1,
        sign(signedmessage, &smlen, message, mlen, sk, pk));

    WRAP_FUNC("verify: " CYCLE_TYPE "\n",
        cycles, time0, time1,
        verify(message, &mlen, signedmessage, smlen, sk->pkbytes, pk));
}