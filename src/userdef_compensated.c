/* The Intel compiler enables unsafe floating-point optimizations by default... */
#ifdef __INTEL_COMPILER
#pragma float_control (precise, on)
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <math.h>

#include <mpi.h>

#include "kahanmpi.h"

#include "state.h"

void KahanMPI_Reduce_compensated(void * in, void * out, int * count, MPI_Datatype * type)
{
    if (*count % 2 != 0) {
        printf("count cannot be odd (%d)\n", *count);
        PMPI_Abort(MPI_COMM_WORLD, 1);
    }
    int n = (*count)/2;

    if (*type == MPI_FLOAT) {
        const float * restrict fin  = (const float*)in;
              float * restrict fout = (float*)out;

        for (int i=0; i<n; ++i) {
            /* simple summation:
             * fout[i] += fin[i]; */
            float y   = fin[i] - fin[i+n];
            float t   = fout[i] + y;
            fout[i+n] = (t - fin[i]) - y;
            fout[i]   = t;
        }
    }

    return;
}

int KahanMPI_Reduce_userdef_compensated(const void *sendbuf, void *recvbuf, int count,
                                       MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm)
{
    int rc = MPI_SUCCESS;

    int typesize = 0;
    rc = PMPI_Type_size(datatype, &typesize);
    if (rc != MPI_SUCCESS) return rc;

    /* We cannot use a user-defined type here because MPI_User_function only takes one datatype argument
     * and there is no reasonable way to ensure the displacement between {send,recv}buf and {send,recv}aux
     * is the same.  Thus, we have to copy in and out of temporaries that are 2x what would otherwise be necessary. */

    MPI_Aint bytes = (MPI_Aint)typesize * (MPI_Aint)count * (MPI_Aint)2;

    void * in;
    void * out;

    rc = PMPI_Alloc_mem(bytes, MPI_INFO_NULL, &in);
    if (rc != MPI_SUCCESS) return rc;

    rc = PMPI_Alloc_mem(bytes, MPI_INFO_NULL, &out);
    if (rc != MPI_SUCCESS) return rc;

    memcpy(in, sendbuf, count*typesize);
    memset(&(in[count]), 0, count*typesize);

    /* DO IT */

    memcpy(recvbuf, out, count*typesize);

    return MPI_ERR_INTERN;
}

#if 0

int KahanMPI_Reduce_scatter_userdef_compensated(const void *sendbuf, void *recvbuf, const int recvcounts[],
                                   MPI_Datatype datatype, MPI_Op op, MPI_Comm comm)
{
    return MPI_ERR_INTERN;
}

int KahanMPI_Reduce_scatter_block_userdef_compensated(const void *sendbuf, void *recvbuf, const int recvcount,
                                         MPI_Datatype datatype, MPI_Op op, MPI_Comm comm)
{
    return MPI_ERR_INTERN;
}

int KahanMPI_Ireduce_userdef_compensated(const void *sendbuf, void *recvbuf, int count,
                            MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm, MPI_Request * request)
{
    return MPI_ERR_INTERN;
}

int KahanMPI_Iallreduce_userdef_compensated(const void *sendbuf, void *recvbuf, int count,
                               MPI_Datatype datatype, MPI_Op op, MPI_Comm comm, MPI_Request * request)
{
    return MPI_ERR_INTERN;
}

int KahanMPI_Ireduce_scatter_userdef_compensated(const void *sendbuf, void *recvbuf, const int recvcounts[],
                                    MPI_Datatype datatype, MPI_Op op, MPI_Comm comm, MPI_Request * request)
{
    return MPI_ERR_INTERN;
}

int KahanMPI_Ireduce_scatter_block_userdef_compensated(const void *sendbuf, void *recvbuf, const int recvcount,
                                          MPI_Datatype datatype, MPI_Op op, MPI_Comm comm, MPI_Request * request)
{
    return MPI_ERR_INTERN;
}

int KahanMPI_Reduce_local_userdef_compensated(const void *inbuf, void *inoutbuf, int count, MPI_Datatype datatype, MPI_Op op)
{
    return MPI_ERR_INTERN;
}

#endif
