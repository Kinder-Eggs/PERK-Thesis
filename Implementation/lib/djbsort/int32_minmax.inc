#define int32_MINMAX(a,b) \
do { \
  int64_t temp1 = 0; \
  int64_t ae = (int64_t) a; \
  int64_t be = (int64_t) b; \
  __asm__( \
    "cmp %0,%1\n\t" \
    "mov %2,%0\n\t" \
    "csel %0,%1,%0,GT\n\t" \
    "csel %1,%2,%1,GT\n\t" \
    : "+r"(ae), "+r"(be), "=r"(temp1) \
    : \
    : "cc" \
  ); \
  a = (int32_t) ae; \
  b = (int32_t) be; \
} while(0)
