#include <mpi.h>
#include <math.h>

#include "kahanmpi.h"
#include "kahansum.h"

int KahanMPI_Reduce_local(const void *inbuf, void *inoutbuf, int count, MPI_Datatype datatype, MPI_Op op)
{
    return MPI_ERR_INTERN;
}

/* if root<0, the semantic is allreduce */
int KahanMPI_Reduce(const void *sendbuf, void *recvbuf, int count,
                    MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm)
{
    int rc = MPI_SUCCESS;

    int typesize = 0;
    rc = MPI_Type_size(datatype, &typesize);
    if (rc != MPI_SUCCESS) return rc;

    int commsize = 1;
    rc = MPI_Comm_size(comm, &commsize);
    if (rc != MPI_SUCCESS) return rc;

    MPI_Aint bytes = (MPI_Aint)typesize * (MPI_Aint)count * (MPI_Aint)commsize;

    int commrank = 0;
    rc = MPI_Comm_rank(comm, &commrank);
    if (rc != MPI_SUCCESS) return rc;

    if (datatype == MPI_FLOAT) {

        /* only used at root */
        float * temp;

        /* allocate buffer for summation only at root */
        if (commrank==abs(root)) {
            rc = MPI_Alloc_mem(bytes, MPI_INFO_NULL, &temp);
            if (rc != MPI_SUCCESS) return rc;
        }

        /* gather all inputs to root */
        rc = MPI_Gather(sendbuf, count, datatype, temp, count, datatype, abs(root), comm);
        if (rc != MPI_SUCCESS) return rc;

        /* perform summation only at root */
        if (commrank==abs(root)) {
            KahanFloatWrapper(commsize, count, temp, recvbuf);
        }

        /* bcast to achieve allreduce semantic */
        if (root<0) {
            rc = MPI_Bcast(recvbuf, count, datatype, abs(root), comm);
            if (rc != MPI_SUCCESS) return rc;
        }

        /* free buffer for summation only at root */
        if (commrank==abs(root)) {
            rc = MPI_Free_mem(temp);
            if (rc != MPI_SUCCESS) return rc;
        }

        return MPI_SUCCESS;

    /*
    } else if (datatype == MPI_DOUBLE) {
    } else if (datatype == MPI_LONG_DOUBLE) {
    */
    }

    return MPI_ERR_INTERN;
}

int KahanMPI_Reduce_scatter(const void *sendbuf, void *recvbuf, const int recvcounts[],
                            MPI_Datatype datatype, MPI_Op op, MPI_Comm comm)
{
    return MPI_ERR_INTERN;
}

int KahanMPI_Reduce_scatter_block(const void *sendbuf, void *recvbuf, const int recvcount,
                                  MPI_Datatype datatype, MPI_Op op, MPI_Comm comm)
{
    return MPI_ERR_INTERN;
}

int KahanMPI_Ireduce(const void *sendbuf, void *recvbuf, int count,
                     MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm, MPI_Request * request)
{
    return MPI_ERR_INTERN;
}

int KahanMPI_Iallreduce(const void *sendbuf, void *recvbuf, int count,
                        MPI_Datatype datatype, MPI_Op op, MPI_Comm comm, MPI_Request * request)
{
    return MPI_ERR_INTERN;
}

int KahanMPI_Ireduce_scatter(const void *sendbuf, void *recvbuf, const int recvcounts[],
                             MPI_Datatype datatype, MPI_Op op, MPI_Comm comm, MPI_Request * request)
{
    return MPI_ERR_INTERN;
}

int KahanMPI_Ireduce_scatter_block(const void *sendbuf, void *recvbuf, const int recvcount,
                                   MPI_Datatype datatype, MPI_Op op, MPI_Comm comm, MPI_Request * request)
{
    return MPI_ERR_INTERN;
}
