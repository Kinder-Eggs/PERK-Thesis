#include <stdint.h>
#include <stddef.h>
#include "../include/parameters.h"
#include "../include/compute.h"
#include "../lib/djbsort/djbsort.h"


static inline int16_t caddq(int16_t a) {
    a += (a >> 15U) & Q;
    return a;
}


void vectorMult(uint16_t* out, const uint16_t scalar, const uint16_t* in, size_t size) {
    for(int i = 0; i < size; i++)
        out[i] = barret_reduce32((uint32_t)scalar * (uint32_t)in[i]);
}


void vectorAdd(uint16_t* out, const uint16_t* vec1, const uint16_t* vec2, size_t size) {
    for(int i = 0; i < size; i++)
        out[i] = caddq((int16_t)((int16_t)vec1[i] + (int16_t)vec2[i] - Q));
}


void vectorSub(uint16_t* out, const uint16_t* vec1, const uint16_t* vec2, size_t size) {
    for(int i = 0; i < size; i++)
        out[i] = caddq((int16_t)((int16_t)vec1[i] - (int16_t)vec2[i]));
}


void vectorPermute(uint16_t out[N1], const uint8_t perm[N1], const uint16_t in[N1]) {
    uint32_t buffer[N1];
    int i;
    for(i = 0; i < N1; i++)
        buffer[i] = (((uint32_t)perm[i]) << 16) | in[i];

    uint32_sort(buffer, N1);
    for(i = 0; i < N1; i++)
        out[i] = (uint16_t)(buffer[i]);
}


void genVect(uint16_t out[N1], uint8_t pis[N2][N1], uint16_t v[N2][N1], uint8_t pi[N1]) {
    uint16_t tmp[N1];
    uint8_t permut[N1];

    computeComposeInv(permut, pi, pis[0]);
    vectorPermute(out, permut, v[0]);
    for(int i = 1; i < N2-1; i++) {
        computeComposeInv(permut, permut, pis[i]);
        vectorPermute(tmp, permut, v[i]);
        vectorAdd(out, out, tmp, N1);
    }
    vectorAdd(out, out, v[N2-1], N1);
}