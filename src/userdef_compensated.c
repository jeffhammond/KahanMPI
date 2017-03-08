/* The Intel compiler enables unsafe floating-point optimizations by default... */
#ifdef __INTEL_COMPILER
#pragma float_control (precise, on)
#endif

#include <stdio.h>
#include <stdlib.h>
#include <limits.h> /* INT_MAX */
#include <string.h>

#include <math.h>

#include <mpi.h>

#include "kahanmpi.h"

#include "state.h"

void KahanMPI_Compensated_sum(void * in, void * out, int * count, MPI_Datatype * type)
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
    } else if (*type == MPI_DOUBLE) {
        const double * restrict fin  = (const double*)in;
              double * restrict fout = (double*)out;

        for (int i=0; i<n; ++i) {
            /* simple summation:
             * fout[i] += fin[i]; */
            double y   = fin[i] - fin[i+n];
            double t   = fout[i] + y;
            fout[i+n] = (t - fin[i]) - y;
            fout[i]   = t;
        }
    } else if (*type == MPI_LONG_DOUBLE) {
        const long double * restrict fin  = (const long double*)in;
              long double * restrict fout = (long double*)out;

        for (int i=0; i<n; ++i) {
            /* simple summation:
             * fout[i] += fin[i]; */
            long double y   = fin[i] - fin[i+n];
            long double t   = fout[i] + y;
            fout[i+n] = (t - fin[i]) - y;
            fout[i]   = t;
        }
    }

    return;
}

/* TODO Create and save the Kahan summation MPI_Op during initialization */

int KahanMPI_Reduce_userdef_compensated(const void *sendbuf, void *recvbuf, int count,
                                       MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm)
{
    int rc = MPI_SUCCESS;

    /* SUM is the only operation supported. */
    if (op != MPI_SUM) return MPI_ERR_INTERN;

    /* See KahanMPI_Compensated_sum for supported types. */
    if ( (datatype != MPI_FLOAT) ||
         (datatype != MPI_DOUBLE) ||
         (datatype != MPI_LONG_DOUBLE) ) return MPI_ERR_INTERN;

    /* If 2*count > INT_MAX, we will encounter overflow.
     * We can fix this later with BigMPI techniques... */
    if (count > (INT_MAX/2)) {
        printf("count %d is too large - will overflow)\n", count);
        PMPI_Abort(MPI_COMM_WORLD, 1);
    }

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

    /* Copy sendbuf to the first half of buffer */
    memcpy(in, sendbuf, count*typesize);
    /* Initialize the second half of buffer to (positive) zero */
    memset(&(in[(count+1)*typesize]), 0, count*typesize);

    /* Create the user-defined datatype */

    MPI_User_function * kahansum = KahanMPI_Compensated_sum;

    MPI_Op kahan_op;
    rc = PMPI_Op_create(kahansum, 1 /* commutative */, &kahan_op);
    if (rc != MPI_SUCCESS) return rc;

    /* Perform the reduction */

    rc = PMPI_Reduce(in, out, 2*count, datatype, kahan_op, root, comm);
    if (rc != MPI_SUCCESS) return rc;

    /* Copy the output back to the user buffer */
    memcpy(recvbuf, out, count*typesize);

    /* Cleanup all the temporary stuff */
    rc = PMPI_Op_free(&kahan_op);
    if (rc != MPI_SUCCESS) return rc;

    rc = PMPI_Free_mem(in);
    if (rc != MPI_SUCCESS) return rc;

    rc= PMPI_Free_mem(out);
    if (rc != MPI_SUCCESS) return rc;

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
