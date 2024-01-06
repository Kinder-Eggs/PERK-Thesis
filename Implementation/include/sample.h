#ifndef SAMPLE
#define SAMPLE

#include <stdint.h>
#include "structs.h"
#include "parameters.h"

void sample_pi(uint8_t pi[N1], uint8_t seed[LAMBDA/8]);
void sample_h(uint16_t h[M][N1], uint8_t seed[LAMBDA/8]);
void sample_x(uint16_t x[T][N1], uint8_t seed[LAMBDA/8]);
void genPiV(instance *instance, uint8_t salt[SECURITY_BYTES*2]);
void genCmts(instance *instance, uint8_t salt[SECURITY_BYTES*2], uint8_t tau);
void genInstanceCmts(instance *instance, uint8_t salt[SECURITY_BYTES*2], int tau, uint8_t pi[N1]);
void genInstanceCmt1(instance *instance, uint8_t salt[SECURITY_BYTES*2], int tau, uint16_t h[M][N1], uint16_t vector[N1]);


#endif