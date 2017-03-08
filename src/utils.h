#ifndef HAVE_UTILS_H
#define HAVE_UTILS_H

#include "kahanmpiconf.h"

#if HAVE_MACRO_VA_ARGS
#include <stdarg.h>
#endif

#include "mybool.h"

char * KahanMPI_Getenv(const char *varname);
bool KahanMPI_Getenv_present(const char *varname);
bool KahanMPI_Getenv_bool(const char *varname, int default_value);
int KahanMPI_Getenv_int(const char *varname, int default_value);
long KahanMPI_Getenv_long(const char *varname, long default_value);

#if HAVE_MACRO_VA_ARGS

void    KahanMPI_Debug_print_impl(const char *func, const char *format, ...);

#define KahanMPI_Debug_print(debug_level, allranks, ...) do {                       \
    if (debug_level >= KahanMPI_Global_State.debug_level) {                         \
        if (allranks || (KahanMPI_Global_State.mpi_world_rank==0)) {                \
            KahanMPI_Debug_print_impl(__func__,__VA_ARGS__);                        \
        }                                                                           \
    }                                                                               \
} while (0)

#else

#define KahanMPI_Debug_print(debug_level, allranks) do {                                                   \
    if (debug_level >= KahanMPI_Global_State.debug_level) {                                                \
        if (allranks || (KahanMPI_Global_State.mpi_world_rank==0)) {                                       \
            printf("[%d] %s: %s\n", KahanMPI_Global_State.mpi_world_rank,  __func__, "no varargs");        \
        }                                                                                                  \
    }                                                                                                      \
} while (0)

#endif

#endif // HAVE_UTILS_H
