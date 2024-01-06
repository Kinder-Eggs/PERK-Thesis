#include <stdlib.h>
#include "parameters.h"
#include "structs.h"

void keyGen(secret_key* sk, public_key* pk);
void sign(unsigned char[32+SIGNATURE_BYTES], size_t*, unsigned char[32], size_t, secret_key*, public_key*);
uint8_t verify(unsigned char[32], size_t*, unsigned char[32+SIGNATURE_BYTES], size_t, uint8_t[PK_BYTES], public_key*);
