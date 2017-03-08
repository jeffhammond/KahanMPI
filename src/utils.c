#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

#include "state.h"

/* Retrieve the value of a environment variable. */
char * KahanMPI_Getenv(const char *varname)
{
  return getenv(varname);
}

/* Ignore value, but determine if envvar is present. */
bool KahanMPI_Getenv_present(const char *varname)
{
  const char *var = getenv(varname);
  return (var==NULL ? false : true);
}

/* Retrieve the value of a boolean environment variable. */
bool KahanMPI_Getenv_bool(const char *varname, int default_value)
{
  const char *var = getenv(varname);

  if (var == NULL) return default_value;

  if (var[0] == 'T' || var[0] == 't' || var[0] == '1' || var[0] == 'y' || var[0] == 'Y') {
    return true;
  } else {
    return false;
  }
}

/* Retrieve the value of an integer environment variable. */
int KahanMPI_Getenv_int(const char *varname, int default_value)
{
  const char *var = getenv(varname);
  if (var) return atoi(var);
  return default_value;
}

/* Retrieve the value of an long integer environment variable. */
long KahanMPI_Getenv_long(const char *varname, long default_value)
{
  const char *var = getenv(varname);
  if (var) return atoi(var);
  return default_value;
}

/* Print a debugging message. */
void KahanMPI_Debug_print_impl(const char *func, const char *format, ...)
{
    va_list etc;
    int  disp = 0;
    char string[500];

    disp += snprintf(string, 500, "[%d] %s: ", KahanMPI_Global_State.mpi_world_rank, func);
    va_start(etc, format);
    disp += vsnprintf(string+disp, 500-disp, format, etc);
    va_end(etc);

    fprintf(stderr, "%s", string);
}
