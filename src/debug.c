/*
 * From ARMCI-MPI.  Same license.
 * Copyright (C) 2010. Argonne National Laboratory.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <mpi.h>

#include "debug.h"

/* Set the default debugging message classes to enable.
 */
unsigned DEBUG_CATS_ENABLED = DEBUG_CAT_NONE;

/* Print an assertion failure message and abort the program. */
void KahanMPI_Assert_fail(const char *expr, const char *msg, const char *file, int line, const char *func)
{
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (msg == NULL)
    fprintf(stderr, "[%d] ARMCI assert fail in %s() [%s:%d]: \"%s\"\n", rank, func, file, line, expr);
  else
    fprintf(stderr, "[%d] ARMCI assert fail in %s() [%s:%d]: \"%s\"\n"
                    "[%d] Message: \"%s\"\n", rank, func, file, line, expr, rank, msg);

#if HAVE_EXECINFO_H
  {
#include <execinfo.h>

    const int SIZE = 100;
    int    j, nframes;
    void  *frames[SIZE];
    char **symbols;

    nframes = backtrace(frames, SIZE);
    symbols = backtrace_symbols(frames, nframes);

    if (symbols == NULL)
      perror("Backtrace failure");

    fprintf(stderr, "[%d] Backtrace:\n", rank);
    for (j = 0; j < nframes; j++)
      fprintf(stderr, "[%d]  %2d - %s\n", rank, nframes-j-1, symbols[j]);

    free(symbols);
  }
#endif

  fflush(NULL);
  {
    double stall = MPI_Wtime();
    while (MPI_Wtime() - stall < 1) ;
  }
  MPI_Abort(MPI_COMM_WORLD, -1);
}


/* Print a debugging message. */
void KahanMPI_Dbg_print_impl(const char *func, const char *format, ...)
{
  va_list etc;
  int disp = 0;
  char string[500];

  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  disp += snprintf(string, 500, "[%d] %s: ", rank, func);
  va_start(etc, format);
  disp += vsnprintf(string+disp, 500-disp, format, etc);
  va_end(etc);

  fprintf(stderr, "%s", string);
}


/* Print an ARMCI warning message. */
void KahanMPI_Warning(const char *fmt, ...)
{
  va_list etc;
  int disp = 0;
  char string[500];

  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  disp += snprintf(string, 500, "[%d] ARMCI Warning: ", rank);
  va_start(etc, fmt);
  disp += vsnprintf(string+disp, 500-disp, fmt, etc);
  va_end(etc);

  fprintf(stderr, "%s", string);
  fflush(NULL);
}

void KahanMPI_Error_impl(const char *file, const int line, const char *func, const char *msg, ...)
{
  va_list ap;
  int disp = 0;
  char string[500];

  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  va_start(ap, msg);
  disp += vsnprintf(string, 500, msg, ap);
  va_end(ap);

  fprintf(stderr, "[%d] KahanMPI Internal error in %s (%s:%d)\n[%d] Message: %s\n", rank,
  func, file, line, rank, string);
  MPI_Abort(MPI_COMM_WORLD, 100);
}

