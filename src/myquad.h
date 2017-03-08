#ifndef HAVE_MYQUAD_H
#define HAVE_MYQUAD_H

#if HAVE_LIBQUADMATH
#include <quadmath.h>
#endif

#if HAVE_GCC_FLOAT128
typedef __float128 quad;
#define HAVE_QUAD_TYPE 1
#elif HAVE_INTEL_QUAD
typedef _Quad quad;
#define HAVE_QUAD_TYPE 1
#endif

#endif // HAVE_MYQUAD_H
