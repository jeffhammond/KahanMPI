/*
 * Copyright (C) 2014. See LICENSE in top-level directory.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <float.h>
#include <mpi.h>

int main(int argc, char ** argv)
{
    MPI_Init(&argc, &argv);

    int np = 1;
    MPI_Comm_size(MPI_COMM_WORLD, &np);

    int me = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &me);

    int root = 0;

    int n = (argc>1) ? atoi(argv[1]) : 1000;

    float * in1 = calloc(n,sizeof(float));
    float * in2 = calloc(n,sizeof(float));
    float * in3 = calloc(n,sizeof(float));

    float * out1 = calloc(n,sizeof(float));
    float * out2 = calloc(n,sizeof(float));
    float * out3 = calloc(n,sizeof(float));

    float big = 1000.0f;
    float small = FLT_EPSILON;
    float value;

    /* stick the huge value in the beginning */
    value = (me==0) ? big : small;
    for (int i=0; i<n; ++i) in1[i] = value;
    //for (int i=0; i<n; ++i) printf("%d: in1[%d]=%e\n", me, i, in1[i]);
    fflush(stdout);

    /* stick the huge value in the end */
    value = (me==(np-1)) ? big : small;
    for (int i=0; i<n; ++i) in2[i] = value;
    //for (int i=0; i<n; ++i) printf("%d: in3[%d]=%e\n", me, i, in2[i]);
    fflush(stdout);

    /* stick the huge value at the midpoint */
    value = (me==(np/2)) ? big : small;
    for (int i=0; i<n; ++i) in3[i] = value;
    //for (int i=0; i<n; ++i) printf("%d: in3[%d]=%e\n", me, i, in3[i]);
    fflush(stdout);

    for (int i=0; i<n; ++i) out1[i] = 0.0f;
    for (int i=0; i<n; ++i) out2[i] = 0.0f;
    for (int i=0; i<n; ++i) out3[i] = 0.0f;

    fflush(stdout); MPI_Barrier(MPI_COMM_WORLD);

    MPI_Allreduce(in1, out1, n, MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);

    fflush(stdout); MPI_Barrier(MPI_COMM_WORLD);

    MPI_Allreduce(in2, out2, n, MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);

    fflush(stdout); MPI_Barrier(MPI_COMM_WORLD);

    MPI_Allreduce(in3, out3, n, MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);

    fflush(stdout); MPI_Barrier(MPI_COMM_WORLD);

    if (me == 0) {
        for (int i=0; i<n; ++i) printf("%d: RESULT %d %e %e %e\n", me, i, out1[i], out2[i], out3[i]);
        for (int i=0; i<n; ++i) printf("%d: ERRORS %d %e %e %e\n", me, i, out1[i]-out2[i], out2[i]-out3[i], out1[i]-out3[i]);
    }

    fflush(stdout); MPI_Barrier(MPI_COMM_WORLD);

    free(out3);
    free(out2);
    free(out1);
    free(in3);
    free(in2);
    free(in1);

    MPI_Finalize();

    return 0;
}
