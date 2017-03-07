#if HAVE_LIBQUADMATH
#include <quadmath.h>
#endif

#if HAVE_GCC_FLOAT128
typedef __float128 quad;
#elif HAVE_INTEL_QUAD
typedef _Quad quad;
#endif
