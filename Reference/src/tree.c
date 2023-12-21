#include <stdint.h>
#include "../include/parameters.h"
#include "../include/hashing.h"
#include "../lib/XKCP/KeccakHash.h"

void expandTree(uint8_t salt[2*SECURITY_BYTES], uint8_t tree[2*N2-1][SECURITY_BYTES]) {
    Keccak_HashInstance state;

    uint8_t from, idx;
    uint16_t to;
    for(from = 0; from < (N2-1); from++) {
        idx = from;
        to = from * 2 + 1;
        hash_init(&state, salt, &idx, NULL);
        hash_update(&state, tree[from], SECURITY_BYTES);
        hash_final(&state, tree[to], 3);
    }
}


void getPartialTheta(uint8_t partial[THETA_TREE][SECURITY_BYTES], uint8_t theta_tree[2*N2-1][SECURITY_BYTES], uint16_t alpha) {
    uint64_t level, node;
    for(int i = 0; i < THETA_TREE; i++) {
        level = (1U << (i + 1U)) - 1;
        node = (alpha >> (THETA_TREE - 1U - i)) ^ 1U;
        memcpy(partial[i], theta_tree[level + node], SECURITY_BYTES);
    }
}


void expandPartialTree(uint8_t salt[SECURITY_BYTES*2], uint8_t tree[2*N2-1][SECURITY_BYTES], uint8_t theta[THETA_TREE][SECURITY_BYTES], uint16_t alpha) {
    Keccak_HashInstance state;
    uint8_t idx;
    uint32_t i, l, j, n, from, to, missing, right;
    for(i = 0, l = 0, j = 0; i < N2-1; i++, j++) {
        n = 1U << l;
        if(j >= n) {
            l++;
            j = 0;
        }
        from = i;
        idx = i;
        to = i*2+1;
        missing = alpha >> (THETA_TREE - l);
        right = (~alpha >> (THETA_TREE - 1 - l)) & 1;
        if(j == missing)
            memcpy(tree[to + right], theta[l], SECURITY_BYTES);
        else {
            hash_init(&state, salt, &idx, NULL);
            hash_update(&state, tree[from], SECURITY_BYTES);
            hash_final(&state, tree[to], 3);
        }
    }
}
