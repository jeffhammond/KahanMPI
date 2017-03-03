/*
 * Copyright (C) 2014. See LICENSE in top-level directory.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <mpi.h>
#include "kahanmpi.h"

int main(int argc, char ** argv)
{
    MPI_Init(&argc, &argv);

    int n = (argc>1) ? atoi(argv[1]) : 1000;

    int * send = calloc(n*sizeof(int));
    int * recv = calloc(n*sizeof(int));

    for (int i=0; i<n; ++i) send[i] =  1;
    for (int i=0; i<n; ++i) recv[i] = -1;

    int me = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &me);

    MPI_Reduce(send, recv, n, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    int test = (me == 0) ? np : -1;
    for (int i=0; i<n; ++i) assert(recv[i] == test);

    MPI_Barrier(MPI_COMM_WORLD);

    for (int i=0; i<n; ++i) recv[i] = -1;

    MPI_Allreduce(send, recv, n, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    int np = 1;
    MPI_Comm_size(MPI_COMM_WORLD, &np);

    for (int i=0; i<n; ++i) assert(recv[i] == np);

    free(send);
    free(recv);

    MPI_Finalize();

    return 0;
}
