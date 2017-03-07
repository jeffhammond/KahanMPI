/* The Intel compiler enables unsafe floating-point optimizations by default... */
#ifdef __INTEL_COMPILER
#pragma float_control (precise, on)
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <math.h>

#include <mpi.h>

#include "myquad.h"

#include "kahanmpi.h"

#define KAHANMPI_COPY(intype, outtype, count, in, out)      \
do {                                                        \
    size_t n = (size_t)count;                               \
    intype  * pin  = (intype*)in;                           \
    outtype * pout = (outtype*)out;                         \
    for (size_t i=0; i<n; ++i) {                            \
        pout[i] = (intype)pin[i];                           \
    }                                                       \
} while(0)

/* if root<0, the semantic is allreduce */
int KahanMPI_Reduce_promote(const void *sendbuf, void *recvbuf, int count,
                            MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm)
{
    int rc = MPI_SUCCESS;

    int commsize = 1;
    rc = MPI_Comm_size(comm, &commsize);
    if (rc != MPI_SUCCESS) return rc;

    int commrank = 0;
    rc = MPI_Comm_rank(comm, &commrank);
    if (rc != MPI_SUCCESS) return rc;

    if (datatype == MPI_FLOAT) {

        double * in;
        double * out;

        MPI_Aint bytes = (MPI_Aint)sizeof(double) * (MPI_Aint)count;
        rc = MPI_Alloc_mem(bytes, MPI_INFO_NULL, &in);
        if (rc != MPI_SUCCESS) return rc;
        rc = MPI_Alloc_mem(bytes, MPI_INFO_NULL, &out);
        if (rc != MPI_SUCCESS) return rc;

        KAHANMPI_COPY(float, double, count, sendbuf, in);

        rc = PMPI_Reduce(in, out, count, MPI_DOUBLE, op, root, comm);
        if (rc != MPI_SUCCESS) return rc;

        KAHANMPI_COPY(double, float, count, out, recvbuf);

        rc = MPI_Free_mem(out);
        if (rc != MPI_SUCCESS) return rc;
        rc = MPI_Free_mem(in);
        if (rc != MPI_SUCCESS) return rc;

        return MPI_SUCCESS;

    }
#if HAVE_LIBQUADMATH
    else if (datatype == MPI_DOUBLE) {

        quad * in;
        quad * out;

        MPI_Aint bytes = (MPI_Aint)sizeof(quad) * (MPI_Aint)count;
        rc = MPI_Alloc_mem(bytes, MPI_INFO_NULL, &in);
        if (rc != MPI_SUCCESS) return rc;
        rc = MPI_Alloc_mem(bytes, MPI_INFO_NULL, &out);
        if (rc != MPI_SUCCESS) return rc;

        KAHANMPI_COPY(double, count, sendbuf, in);

        rc = PMPI_Reduce(in, out, count, MPI_FLOAT128, op128, root, comm);
        if (rc != MPI_SUCCESS) return rc;

        KAHANMPI_COPY(quad, count, out, recvbuf);

        rc = MPI_Free_mem(out);
        if (rc != MPI_SUCCESS) return rc;
        rc = MPI_Free_mem(in);
        if (rc != MPI_SUCCESS) return rc;

        return MPI_SUCCESS;

    } else if (datatype == MPI_LONG_DOUBLE) {

        quad * in;
        quad * out;

        MPI_Aint bytes = (MPI_Aint)sizeof(quad) * (MPI_Aint)count;
        rc = MPI_Alloc_mem(bytes, MPI_INFO_NULL, &in);
        if (rc != MPI_SUCCESS) return rc;
        rc = MPI_Alloc_mem(bytes, MPI_INFO_NULL, &out);
        if (rc != MPI_SUCCESS) return rc;

        KAHANMPI_COPY(long double, count, sendbuf, in);

        rc = PMPI_Reduce(in, out, count, MPI_FLOAT128, op128, root, comm);
        if (rc != MPI_SUCCESS) return rc;

        KAHANMPI_COPY(quad, count, out, recvbuf);

        rc = MPI_Free_mem(out);
        if (rc != MPI_SUCCESS) return rc;
        rc = MPI_Free_mem(in);
        if (rc != MPI_SUCCESS) return rc;

        return MPI_SUCCESS;
    }
#endif

    return MPI_ERR_INTERN;
}

#if 0

int KahanMPI_Reduce_scatter_promote(const void *sendbuf, void *recvbuf, const int recvcounts[],
                                    MPI_Datatype datatype, MPI_Op op, MPI_Comm comm)
{
    return MPI_ERR_INTERN;
}

int KahanMPI_Reduce_scatter_block_promote(const void *sendbuf, void *recvbuf, const int recvcount,
                                          MPI_Datatype datatype, MPI_Op op, MPI_Comm comm)
{
    return MPI_ERR_INTERN;
}

int KahanMPI_Ireduce_promote(const void *sendbuf, void *recvbuf, int count,
                             MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm, MPI_Request * request)
{
    return MPI_ERR_INTERN;
}

int KahanMPI_Iallreduce_promote(const void *sendbuf, void *recvbuf, int count,
                                MPI_Datatype datatype, MPI_Op op, MPI_Comm comm, MPI_Request * request)
{
    return MPI_ERR_INTERN;
}

int KahanMPI_Ireduce_scatter_promote(const void *sendbuf, void *recvbuf, const int recvcounts[],
                                     MPI_Datatype datatype, MPI_Op op, MPI_Comm comm, MPI_Request * request)
{
    return MPI_ERR_INTERN;
}

int KahanMPI_Ireduce_scatter_block_promote(const void *sendbuf, void *recvbuf, const int recvcount,
                                           MPI_Datatype datatype, MPI_Op op, MPI_Comm comm, MPI_Request * request)
{
    return MPI_ERR_INTERN;
}

int KahanMPI_Reduce_local_promote(const void *inbuf, void *inoutbuf, int count, MPI_Datatype datatype, MPI_Op op)
{
    return MPI_ERR_INTERN;
}

#endif
