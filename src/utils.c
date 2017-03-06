#include <stdlib.h>

/* Retrieve the value of a environment variable.  */
char * KahanMPI_Getenv(const char *varname)
{
  return getenv(varname);
}

/* Retrieve the value of a boolean environment variable.  */
int KahanMPI_Getenv_bool(const char *varname, int default_value)
{
  const char *var = getenv(varname);

  if (var == NULL) return default_value;

  if (var[0] == 'T' || var[0] == 't' || var[0] == '1' || var[0] == 'y' || var[0] == 'Y') {
    return 1;
  } else {
    return 0;
  }
}

/* Retrieve the value of an integer environment variable.  */
int KahanMPI_Getenv_int(const char *varname, int default_value)
{
  const char *var = getenv(varname);
  if (var) return atoi(var);
  return default_value;
}

/* Retrieve the value of an long integer environment variable.  */
long KahanMPI_Getenv_long(const char *varname, long default_value)
{
  const char *var = getenv(varname);
  if (var) return atoi(var);
  return default_value;
}
