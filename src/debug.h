/*
 * From ARMCI-MPI.  Same license.
 * Copyright (C) 2010. Argonne National Laboratory.
 */

#ifndef HAVE_DEBUG_H
#define HAVE_DEBUG_H

#include <kahanmpiconf.h>
#include <stdarg.h>

enum debug_cats_e {
  DEBUG_CAT_ALL        =  -1,
  DEBUG_CAT_NONE       =   0,
  DEBUG_CAT_MEM_REGION = 0x1,  // 2^0
  DEBUG_CAT_ALLOC      = 0x2,  // 2^1
  DEBUG_CAT_MUTEX      = 0x4,  // 2^2
  DEBUG_CAT_GROUPS     = 0x8,  // 2^3
  DEBUG_CAT_CTREE      = 0x10, // 2^4, ...
  DEBUG_CAT_IOV        = 0x20
};

/* A logical OR of the debug message categories that are enabled.
 */
extern  unsigned DEBUG_CATS_ENABLED;

void    KahanMPI_Assert_fail(const char *expr, const char *msg, const char *file, int line, const char *func);
#define KahanMPI_Assert(EXPR)          do { if (unlikely(!(EXPR))) KahanMPI_Assert_fail(#EXPR, NULL, __FILE__, __LINE__, __func__); } while(0)
#define KahanMPI_Assert_msg(EXPR, MSG) do { if (unlikely(!(EXPR))) KahanMPI_Assert_fail(#EXPR, MSG,  __FILE__, __LINE__, __func__); } while(0)

#define DEBUG_CAT_ENABLED(X) (DEBUG_CATS_ENABLED & (X))
void    KahanMPI_Dbg_print_impl(const char *func, const char *format, ...);
#define KahanMPI_Dbg_print(CAT,...) do { if (DEBUG_CAT_ENABLED(CAT)) KahanMPI_Dbg_print_impl(__func__,__VA_ARGS__); } while (0)

#define KahanMPI_Error(...) KahanMPI_Error_impl(__FILE__,__LINE__,__func__,__VA_ARGS__)
void    KahanMPI_Error_impl(const char *file, const int line, const char *func, const char *msg, ...);
void    KahanMPI_Warning(const char *fmt, ...);

#endif // HAVE_DEBUG_H
