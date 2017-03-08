#ifndef HAVE_UTILS_H
#define HAVE_UTILS_H

#include "kahanmpiconf.h"

#include "mybool.h"

char * KahanMPI_Getenv(const char *varname);
bool KahanMPI_Getenv_present(const char *varname);
bool KahanMPI_Getenv_bool(const char *varname, int default_value);
int KahanMPI_Getenv_int(const char *varname, int default_value);
long KahanMPI_Getenv_long(const char *varname, long default_value);

#endif // HAVE_UTILS_H
