#ifndef TREE
#define TREE

#include <stdint.h>
#include "parameters.h"

void expandTree(uint8_t salt[2*SECURITY_BYTES], uint8_t tree[2*N2-1][SECURITY_BYTES]);
void getPartialTheta(uint8_t partial[THETA_TREE][SECURITY_BYTES], uint8_t theta_tree[2*N2-1][SECURITY_BYTES], uint16_t alpha);
void expandPartialTree(uint8_t salt[SECURITY_BYTES*2], uint8_t tree[2*N2-1][SECURITY_BYTES], uint8_t theta[THETA_TREE][SECURITY_BYTES], uint16_t alpha);


#endif