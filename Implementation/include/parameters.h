#ifndef PARAMS
#define PARAMS

#define Q 1021
#define Q_MASK ((1U << 10) - 1)

#ifdef FAST13
    #define LAMBDA 128
    #define N1 79
    #define N1_BITS_TWICE 13
    #define M 35
    #define T 3
    #define N2 32
    #define THETA_TREE 5
    #define TAU 30
    #define SK_SIZE 16
    #define PK_SIZE 150
    #define SECURITY_BYTES 16
    #define SHAKE_BLOCK_SIZE 168
    #define Keccak_HashInitialize_SHAKE Keccak_HashInitialize_SHAKE128  // SHAKE128
    #define Keccak_HashInitialize_SHA3  Keccak_HashInitialize_SHA3_256

#elif FAST15
    #define LAMBDA 128
    #define N1 83
    #define N1_BITS_TWICE 13
    #define M 36
    #define T 5
    #define N2 32
    #define THETA_TREE 5
    #define TAU 28
    #define SK_SIZE 16
    #define PK_SIZE 240
    #define SECURITY_BYTES 16
    #define SHAKE_BLOCK_SIZE 168
    #define Keccak_HashInitialize_SHAKE Keccak_HashInitialize_SHAKE128  // SHAKE128
    #define Keccak_HashInitialize_SHA3  Keccak_HashInitialize_SHA3_256

#elif SHORT13
    #define LAMBDA 128
    #define N1 79
    #define N1_BITS_TWICE 13
    #define M 35
    #define T 3
    #define N2 256
    #define THETA_TREE 8
    #define TAU 20
    #define SK_SIZE 16
    #define PK_SIZE 150
    #define SECURITY_BYTES 16
    #define SHAKE_BLOCK_SIZE 168
    #define Keccak_HashInitialize_SHAKE Keccak_HashInitialize_SHAKE128  // SHAKE128
    #define Keccak_HashInitialize_SHA3  Keccak_HashInitialize_SHA3_256

#elif SHORT15
    #define LAMBDA 128
    #define N1 83
    #define N1_BITS_TWICE 13
    #define M 36
    #define T 5
    #define N2 256
    #define THETA_TREE 8
    #define TAU 18
    #define SK_SIZE 16
    #define PK_SIZE 240
    #define SECURITY_BYTES 16
    #define SHAKE_BLOCK_SIZE 168
    #define Keccak_HashInitialize_SHAKE Keccak_HashInitialize_SHAKE128  // SHAKE128
    #define Keccak_HashInitialize_SHA3  Keccak_HashInitialize_SHA3_256

#elif FAST33
    #define LAMBDA 192
    #define N1 112
    #define N1_BITS_TWICE 14
    #define M 54
    #define T 3
    #define N2 32
    #define THETA_TREE 5
    #define TAU 46
    #define SK_SIZE 24
    #define PK_SIZE 230
    #define SECURITY_BYTES 24
    #define SHAKE_BLOCK_SIZE 136
    #define Keccak_HashInitialize_SHAKE Keccak_HashInitialize_SHAKE256  // SHAKE256
    #define Keccak_HashInitialize_SHA3  Keccak_HashInitialize_SHA3_384

#elif FAST35
    #define LAMBDA 192
    #define N1 116
    #define N1_BITS_TWICE 14
    #define M 55
    #define T 5
    #define N2 32
    #define THETA_TREE 5
    #define TAU 43
    #define SK_SIZE 24
    #define PK_SIZE 370
    #define SECURITY_BYTES 24
    #define SHAKE_BLOCK_SIZE 136
    #define Keccak_HashInitialize_SHAKE Keccak_HashInitialize_SHAKE256  // SHAKE256
    #define Keccak_HashInitialize_SHA3  Keccak_HashInitialize_SHA3_384

#elif SHORT33
    #define LAMBDA 192
    #define N1 112
    #define N1_BITS_TWICE 14
    #define M 54
    #define T 3
    #define N2 256
    #define THETA_TREE 8
    #define TAU 31
    #define SK_SIZE 24
    #define PK_SIZE 230
    #define SECURITY_BYTES 24
    #define SHAKE_BLOCK_SIZE 136
    #define Keccak_HashInitialize_SHAKE Keccak_HashInitialize_SHAKE256  // SHAKE256
    #define Keccak_HashInitialize_SHA3  Keccak_HashInitialize_SHA3_384

#elif SHORT35
    #define LAMBDA 192
    #define N1 116
    #define N1_BITS_TWICE 14
    #define M 55
    #define T 5
    #define N2 256
    #define THETA_TREE 8
    #define TAU 28
    #define SK_SIZE 24
    #define PK_SIZE 370
    #define SECURITY_BYTES 24
    #define SHAKE_BLOCK_SIZE 136
    #define Keccak_HashInitialize_SHAKE Keccak_HashInitialize_SHAKE256  // SHAKE256
    #define Keccak_HashInitialize_SHA3  Keccak_HashInitialize_SHA3_384

#elif FAST53
    #define LAMBDA 256
    #define N1 146
    #define N1_BITS_TWICE 15
    #define M 75
    #define T 3
    #define N2 32
    #define THETA_TREE 5
    #define TAU 61
    #define SK_SIZE 32
    #define PK_SIZE 310
    #define SECURITY_BYTES 32
    #define SHAKE_BLOCK_SIZE 136
    #define Keccak_HashInitialize_SHAKE Keccak_HashInitialize_SHAKE256  // SHAKE256
    #define Keccak_HashInitialize_SHA3  Keccak_HashInitialize_SHA3_512

#elif FAST55
    #define LAMBDA 256
    #define N1 150
    #define N1_BITS_TWICE 15
    #define M 76
    #define T 5
    #define N2 32
    #define THETA_TREE 5
    #define TAU 57
    #define SK_SIZE 32
    #define PK_SIZE 510
    #define SECURITY_BYTES 32
    #define SHAKE_BLOCK_SIZE 136
    #define Keccak_HashInitialize_SHAKE Keccak_HashInitialize_SHAKE256  // SHAKE256
    #define Keccak_HashInitialize_SHA3  Keccak_HashInitialize_SHA3_512

#elif SHORT53
    #define LAMBDA 256
    #define N1 146
    #define N1_BITS_TWICE 15
    #define M 75
    #define T 3
    #define N2 256
    #define THETA_TREE 8
    #define TAU 41
    #define SK_SIZE 32
    #define PK_SIZE 310
    #define SECURITY_BYTES 32
    #define SHAKE_BLOCK_SIZE 136
    #define Keccak_HashInitialize_SHAKE Keccak_HashInitialize_SHAKE256  // SHAKE256
    #define Keccak_HashInitialize_SHA3  Keccak_HashInitialize_SHA3_512

#else //SHORT55
    #define LAMBDA 256
    #define N1 150
    #define N1_BITS_TWICE 15
    #define M 76
    #define T 5
    #define N2 256
    #define THETA_TREE 8
    #define TAU 37
    #define SK_SIZE 32
    #define PK_SIZE 510
    #define SECURITY_BYTES 32
    #define SHAKE_BLOCK_SIZE 136
    #define Keccak_HashInitialize_SHAKE Keccak_HashInitialize_SHAKE256  // SHAKE256
    #define Keccak_HashInitialize_SHA3  Keccak_HashInitialize_SHA3_512

#endif  // Perk version


#define N_MASK ((1U << THETA_TREE) - 1)
#define SIGNATURE_BYTES                                                                                     \
    (2*SECURITY_BYTES + (2 * 2*SECURITY_BYTES) + (2*SECURITY_BYTES + SECURITY_BYTES * THETA_TREE) * TAU +   \
     ((TAU * N1 * 10 + 7) / 8) +                                                                            \
     ((TAU * N1 * N1_BITS_TWICE / 2 + 7) / 8))
#define PK_BYTES (SECURITY_BYTES + ((M*10*T+7)/8))

#endif  // ifndef