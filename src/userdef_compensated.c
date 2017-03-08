/* The Intel compiler enables unsafe floating-point optimizations by default... */
#ifdef __INTEL_COMPILER
#pragma float_control (precise, on)
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <math.h>
#include <limits.h> /* INT_MAX */

#include <mpi.h>

#include "kahanmpi.h"

#include "state.h"
#include "debug.h"

void KahanMPI_Compensated_sum(void * in, void * out, int * count, MPI_Datatype * type)
{
    if (*count % 2 != 0) {
        KahanMPI_Error("count cannot be odd (%d)\n", *count);
    }

    int n = (*count)/2;

    if (*type == MPI_FLOAT) {
        const float * restrict fin  = (const float*)in;
              float * restrict fout = (float*)out;

        for (int i=0; i<n; ++i) {
#if 1
            fout[i]   += fin[i];
            fout[i+n]  = 0;
#else
            /* THIS IS WRONG */
            float y   = fin[i] - fin[i+n];
            float t   = fout[i] + y;
            fout[i+n] = (t - fin[i]) - y;
            fout[i]   = t;
#endif
        }
    } else if (*type == MPI_DOUBLE) {
        const double * restrict fin  = (const double*)in;
              double * restrict fout = (double*)out;

        for (int i=0; i<n; ++i) {
#if 1
            fout[i]   += fin[i];
            fout[i+n]  = 0;
#else
            /* THIS IS WRONG */
            double y   = fin[i] - fin[i+n];
            double t   = fout[i] + y;
            fout[i+n] = (t - fin[i]) - y;
            fout[i]   = t;
#endif
        }
    } else if (*type == MPI_LONG_DOUBLE) {
        const long double * restrict fin  = (const long double*)in;
              long double * restrict fout = (long double*)out;

        for (int i=0; i<n; ++i) {
#if 1
            fout[i]   += fin[i];
            fout[i+n]  = 0;
#else
            /* THIS IS WRONG */
            long double y   = fin[i] - fin[i+n];
            long double t   = fout[i] + y;
            fout[i+n] = (t - fin[i]) - y;
            fout[i]   = t;
#endif
        }
    }
}

/* TODO Create and save the Kahan summation MPI_Op during initialization */

int KahanMPI_Reduce_userdef_compensated(const void *sendbuf, void *recvbuf, int count,
                                       MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm)
{
    /* SUM is the only operation supported. */
    if (op != MPI_SUM) {
        KahanMPI_Warning("Only MPI_SUM is supported!\n");
        return MPI_ERR_INTERN;
    }

    /* See KahanMPI_Compensated_sum for supported types. */
    if ( (datatype != MPI_FLOAT) &&
         (datatype != MPI_DOUBLE) &&
         (datatype != MPI_LONG_DOUBLE) ) {
        KahanMPI_Warning("Unsupported datatype (%zu) requested!\n", (size_t)datatype);
        return MPI_ERR_INTERN;
    }

    /* If 2*count > INT_MAX, we will encounter overflow.
     * We can fix this later with BigMPI techniques... */
    if (count > (INT_MAX/2)) {
        KahanMPI_Error("count %d is too large - will overflow)\n", count);
    }

    int typesize = 0;
    PMPI_Type_size(datatype, &typesize);

    /* We cannot use a user-defined type here because MPI_User_function only takes one datatype argument
     * and there is no reasonable way to ensure the displacement between {send,recv}buf and {send,recv}aux
     * is the same.  Thus, we have to copy in and out of temporaries that are 2x what would otherwise be necessary. */

    MPI_Aint bytes = (MPI_Aint)typesize * (MPI_Aint)count * (MPI_Aint)2;

    void * in;
    void * out;

    PMPI_Alloc_mem(bytes, MPI_INFO_NULL, &in);
    PMPI_Alloc_mem(bytes, MPI_INFO_NULL, &out);

    /* Copy sendbuf to the first half of buffer */
    memcpy(in, sendbuf, count*typesize);
    /* Initialize the second half of buffer to (positive) zero */
    memset(&(in[(count+1)*typesize]), 0, count*typesize);

    /* Create the user-defined datatype */

    MPI_User_function * kahansum = KahanMPI_Compensated_sum;

    MPI_Op kahan_op;
    PMPI_Op_create(kahansum, 1 /* commutative */, &kahan_op);

    /* Perform the reduction */

    PMPI_Reduce(in, out, 2*count, datatype, kahan_op, root, comm);

    /* Copy the output back to the user buffer */
    memcpy(recvbuf, out, count*typesize);

    /* Cleanup all the temporary stuff */
    PMPI_Op_free(&kahan_op);
    PMPI_Free_mem(in);
    PMPI_Free_mem(out);

    return MPI_SUCCESS;
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
