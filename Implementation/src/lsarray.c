#include <stdint.h>
#include "../include/parameters.h"


void storeInArray(uint8_t *sb, int i, uint16_t val) {
    val &= 0x3FF;
    int k = (i / 4) * 5;
    switch (i % 4) {
        case 0:
            sb[k + 0] = val;
            sb[k + 1] = val >> 8;
            break;
        case 1:
            sb[k + 1] |= val << 2;
            sb[k + 2] = val >> 6;
            break;
        case 2:
            sb[k + 2] |= val << 4;
            sb[k + 3] = val >> 4;
            break;
        case 3:
            sb[k + 3] |= val << 6;
            sb[k + 4] = val >> 2;
            break;
        default:
            break;
    }
}