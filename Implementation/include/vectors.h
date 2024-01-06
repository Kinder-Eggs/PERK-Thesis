#ifndef VECTORS
#define VECTORS

#include <stdint.h>
#include "parameters.h"

void vectorMult(uint16_t* out, uint16_t scalar, uint16_t* in, size_t size);
void vectorAdd(uint16_t* out, uint16_t* vec1, uint16_t* vec2, size_t size);
void vectorSub(uint16_t* out, const uint16_t* vec1, const uint16_t* vec2, size_t size);
void vectorPermute(uint16_t out[N1], uint8_t perm[N1], uint16_t in[N1]);
void genVect(uint16_t out[N1], uint8_t pis[N2][N1], uint16_t v[N2][N1], uint8_t pi[N1]);


#endif