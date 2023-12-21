#ifndef LSARRAY
#define LSARRAY

#include <stdio.h>
#include <stdint.h>
#include "parameters.h"


void storeInArray(uint8_t *sb, int i, uint16_t val);


static inline void s10bitInArray(uint8_t* sb, int i, uint16_t val) {

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


static inline uint16_t l10bitFromArray(const uint8_t* sb, int i) {
    int k = (i / 4) * 5;
    uint16_t val = 0;
    switch (i % 4) {
        case 0:
            val = sb[k + 0];
            val |= ((uint16_t)sb[k + 1] & 0x3) << 8;
            break;
        case 1:
            val = sb[k + 1] >> 2;
            val |= ((uint16_t)sb[k + 2] & 0xF) << 6;
            break;
        case 2:
            val = sb[k + 2] >> 4;
            val |= ((uint16_t)sb[k + 3] & 0x3F) << 4;
            break;
        case 3:
            val = sb[k + 3] >> 6;
            val |= ((uint16_t)sb[k + 4] & 0xFF) << 2;
            break;
        default:
            break;
    }

    return val;
}


#if (N1_BITS_TWICE == 13)
static inline void s13bitInArray(uint8_t* sb, int i, uint16_t val) {
    val &= 0x1FFF;
    int k = (i * 13) / 8;
    switch (i % 8) {
        case 0:
            sb[k + 0] = val;
            sb[k + 1] = val >> 8;
            break;
        case 1:
            sb[k + 0] |= val << 5;
            sb[k + 1] = val >> 3;
            sb[k + 2] = val >> 11;
            break;
        case 2:
            sb[k + 0] |= val << 2;
            sb[k + 1] = val >> 6;
            break;
        case 3:
            sb[k + 0] |= val << 7;
            sb[k + 1] = val >> 1;
            sb[k + 2] = val >> 9;
            break;
        case 4:
            sb[k + 0] |= val << 4;
            sb[k + 1] = val >> 4;
            sb[k + 2] = val >> 12;
            break;
        case 5:
            sb[k + 0] |= val << 1;
            sb[k + 1] = val >> 7;
            break;
        case 6:
            sb[k + 0] |= val << 6;
            sb[k + 1] = val >> 2;
            sb[k + 2] = val >> 10;
            break;
        case 7:
            sb[k + 0] |= val << 3;
            sb[k + 1] = val >> 5;
            break;
    }
}

/**
 * @brief load a 13 bit value from a byte array at bit position i*13
 *
 * @param sb byte array pointer
 * @param i  position in the byte array
 * @return   uint16_t loaded value
 */
static inline uint16_t l13bitFromArray(const uint8_t* sb, int i) {
    int k = (i * 13) / 8;
    uint16_t val = 0;
    switch (i % 8) {
        case 0:
            val = sb[k + 0];
            val |= ((uint16_t)sb[k + 1] & 0x1F) << 8;
            break;
        case 1:
            val = sb[k + 0] >> 5;
            val |= ((uint16_t)sb[k + 1] & 0xFF) << 3;
            val |= ((uint16_t)sb[k + 2] & 0x03) << 11;
            break;
        case 2:
            val = sb[k + 0] >> 2;
            val |= ((uint16_t)sb[k + 1] & 0x7F) << 6;
            break;
        case 3:
            val = sb[k + 0] >> 7;
            val |= ((uint16_t)sb[k + 1] & 0xFF) << 1;
            val |= ((uint16_t)sb[k + 2] & 0x0F) << 9;
            break;
        case 4:
            val = sb[k + 0] >> 4;
            val |= ((uint16_t)sb[k + 1] & 0xFF) << 4;
            val |= ((uint16_t)sb[k + 2] & 0x01) << 12;
            break;
        case 5:
            val = sb[k + 0] >> 1;
            val |= ((uint16_t)sb[k + 1] & 0x3F) << 7;
            break;
        case 6:
            val = sb[k + 0] >> 6;
            val |= ((uint16_t)sb[k + 1] & 0xFF) << 2;
            val |= ((uint16_t)sb[k + 2] & 0x07) << 10;
            break;
        case 7:
            val = sb[k + 0] >> 3;
            val |= ((uint16_t)sb[k + 1] & 0xFF) << 5;
            break;
    }

    return val;
}

static inline void s2coeffFromArray(uint8_t* sb, int i, uint16_t c0, uint16_t c1) {    
    s13bitInArray(sb, i, (c1 * 90) + c0);
}


static inline void l2coeffFromArray(uint16_t* c0, uint16_t* c1, const uint8_t* sb, int i) {
    uint16_t val = l13bitFromArray(sb, i);
    *c0 = val % 90;
    *c1 = val / 90;
}

#elif (N1_BITS_TWICE == 14)
static inline void s7bitInArray(uint8_t* sb, int i, uint16_t val) {
    val &= 0x7F;
    int k = (i * 7) / 8;
    switch (i % 8) {
        case 0:
            sb[k + 0] = val;
            break;
        case 1:
            sb[k + 0] |= val << 7;
            sb[k + 1] = val >> 1;
            break;
        case 2:
            sb[k + 0] |= val << 6;
            sb[k + 1] = val >> 2;
            break;
        case 3:
            sb[k + 0] |= val << 5;
            sb[k + 1] = val >> 3;
            break;
        case 4:
            sb[k + 0] |= val << 4;
            sb[k + 1] = val >> 4;
            break;
        case 5:
            sb[k + 0] |= val << 3;
            sb[k + 1] = val >> 5;
            break;
        case 6:
            sb[k + 0] |= val << 2;
            sb[k + 1] = val >> 6;
            break;
        case 7:
            sb[k + 0] |= val << 1;
            break;
    }
}

/**
 * @brief load a 7 bit value from a byte array at bit position i*7
 *
 * @param sb byte array pointer
 * @param i  position in the byte array
 * @return   uint8_t loaded value
 */
static inline uint8_t l7bitFromArray(const uint8_t* sb, int i) {
    uint8_t val = 0;
    int k = (i * 7) / 8;
    switch (i % 8) {
        case 0:
            val = (sb[k + 0] & 0x7F);
            break;
        case 1:
            val = sb[k + 0] >> 7;
            val |= (sb[k + 1] & 0x3F) << 1;
            break;
        case 2:
            val = sb[k + 0] >> 6;
            val |= (sb[k + 1] & 0x1F) << 2;
            break;
        case 3:
            val = sb[k + 0] >> 5;
            val |= (sb[k + 1] & 0x0F) << 3;
            break;
        case 4:
            val = sb[k + 0] >> 4;
            val |= (sb[k + 1] & 0x07) << 4;
            break;
        case 5:
            val = sb[k + 0] >> 3;
            val |= (sb[k + 1] & 0x03) << 5;
            break;
        case 6:
            val = sb[k + 0] >> 2;
            val |= (sb[k + 1] & 0x01) << 6;
            break;
        case 7:
            val = sb[k + 0] >> 1;
            break;
        default:
            break;
    }

    return val;
}

static inline void s2coeffFromArray(uint8_t* sb, int i, uint16_t c0, uint16_t c1) {
    s7bitInArray(sb, i * 2 + 0, c0);
    s7bitInArray(sb, i * 2 + 1, c1);
}


static inline void l2coeffFromArray(uint16_t* c0, uint16_t* c1, const uint8_t* sb, int i) {
    *c0 = l7bitFromArray(sb, i * 2 + 0);
    *c1 = l7bitFromArray(sb, i * 2 + 1);
}

#elif (N1_BITS_TWICE == 15)
static inline void s15bitInArray(uint8_t* sb, int i, uint16_t val) {
    val &= 0x7FFF;
    int k = (i * 15) / 8;
    switch (i % 8) {
        case 0:
            sb[k + 0] = val;
            sb[k + 1] = val >> 8;
            break;
        case 1:
            sb[k + 0] |= val << 7;
            sb[k + 1] = val >> 1;
            sb[k + 2] = val >> 9;
            break;
        case 2:
            sb[k + 0] |= val << 6;
            sb[k + 1] = val >> 2;
            sb[k + 2] = val >> 10;
            break;
        case 3:
            sb[k + 0] |= val << 5;
            sb[k + 1] = val >> 3;
            sb[k + 2] = val >> 11;
            break;
        case 4:
            sb[k + 0] |= val << 4;
            sb[k + 1] = val >> 4;
            sb[k + 2] = val >> 12;
            break;
        case 5:
            sb[k + 0] |= val << 3;
            sb[k + 1] = val >> 5;
            sb[k + 2] = val >> 13;
            break;
        case 6:
            sb[k + 0] |= val << 2;
            sb[k + 1] = val >> 6;
            sb[k + 2] = val >> 14;
            break;
        case 7:
            sb[k + 0] |= val << 1;
            sb[k + 1] = val >> 7;
            break;
    }
}

/**
 * @brief load a 15 bit value from a byte array at bit position i*15
 *
 * @param sb byte array pointer
 * @param i  position in the byte array
 * @return   uint16_t loaded value
 */
static inline uint16_t l15bitFromArray(const uint8_t* sb, int i) {
    int k = (i * 15) / 8;
    uint16_t val = 0;
    switch (i % 8) {
        case 0:
            val = sb[k + 0];
            val |= ((uint16_t)sb[k + 1] & 0x7F) << 8;
            break;
        case 1:
            val = sb[k + 0] >> 7;
            val |= ((uint16_t)sb[k + 1] & 0xFF) << 1;
            val |= ((uint16_t)sb[k + 2] & 0x3F) << 9;
            break;
        case 2:
            val = sb[k + 0] >> 6;
            val |= ((uint16_t)sb[k + 1] & 0xFF) << 2;
            val |= ((uint16_t)sb[k + 2] & 0x1F) << 10;
            break;
        case 3:
            val = sb[k + 0] >> 5;
            val |= ((uint16_t)sb[k + 1] & 0xFF) << 3;
            val |= ((uint16_t)sb[k + 2] & 0x0F) << 11;
            break;
        case 4:
            val = sb[k + 0] >> 4;
            val |= ((uint16_t)sb[k + 1] & 0xFF) << 4;
            val |= ((uint16_t)sb[k + 2] & 0x07) << 12;
            break;
        case 5:
            val = sb[k + 0] >> 3;
            val |= ((uint16_t)sb[k + 1] & 0xFF) << 5;
            val |= ((uint16_t)sb[k + 2] & 0x03) << 13;
            break;
        case 6:
            val = sb[k + 0] >> 2;
            val |= ((uint16_t)sb[k + 1] & 0xFF) << 6;
            val |= ((uint16_t)sb[k + 2] & 0x01) << 14;
            break;
        case 7:
            val = sb[k + 0] >> 1;
            val |= ((uint16_t)sb[k + 1] & 0xFF) << 7;
            break;
    }

    return val;
}

static inline void s2coeffFromArray(uint8_t* sb, int i, uint16_t c0, uint16_t c1) {
    s15bitInArray(sb, i, (c1 * 181) + c0);
}


static inline void l2coeffFromArray(uint16_t* c0, uint16_t* c1, const uint8_t* sb, int i) {
    uint16_t val = l15bitFromArray(sb, i);
    *c0 = val % 181;
    *c1 = val / 181;
}
#endif

#endif