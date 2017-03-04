#ifdef __INTEL_COMPILER
#pragma float_control (precise, on)
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <math.h>

#include <mpi.h>

#include "kahanmpi.h"
//#include "kahansum.h"

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

    int commrank = 0;
    rc = MPI_Comm_rank(comm, &commrank);
    if (rc != MPI_SUCCESS) return rc;

    if (datatype == MPI_FLOAT) {

        /* only used at root */
        float * gatherbuf;
        float * reducebuf;

        /* allocate buffer for summation only at root */
        if (commrank==abs(root)) {
            MPI_Aint bytes;

            bytes = (MPI_Aint)typesize * (MPI_Aint)count * (MPI_Aint)commsize;
            rc = MPI_Alloc_mem(bytes, MPI_INFO_NULL, &gatherbuf);
            if (rc != MPI_SUCCESS) return rc;

            bytes = (MPI_Aint)typesize * (MPI_Aint)count;
            rc = MPI_Alloc_mem(bytes, MPI_INFO_NULL, &reducebuf);
            if (rc != MPI_SUCCESS) return rc;
        }

        /* gather all inputs to root */
        rc = MPI_Gather(sendbuf, count, datatype, gatherbuf, count, datatype, abs(root), comm);
        if (rc != MPI_SUCCESS) return rc;

        /* perform summation only at root */
        if (commrank==abs(root)) {
            fflush(stdout);
            for (int j=0; j<count; ++j) {
                float sum = 0.0f;
                float low = 0.0f;
                for (int i=0; i<commsize; ++i) {
                    int index = j+i*count;
                    /* simple summation:
                     * sum += gatherbuf[index]; */
                    float y = gatherbuf[index] - low;
                    float t = sum + y;
                    low = (t - sum) - y;
                    sum = t;
                }
                reducebuf[j] = sum;
            }
            //fflush(stdout);
            memcpy(recvbuf, reducebuf, count * sizeof(float) );
        }

        /* bcast to achieve allreduce semantic */
        if (root<0) {
            rc = MPI_Bcast(recvbuf, count, datatype, abs(root), comm);
            if (rc != MPI_SUCCESS) return rc;
        }

        /* free buffer for summation only at root */
        if (commrank==abs(root)) {
            rc = MPI_Free_mem(gatherbuf);
            if (rc != MPI_SUCCESS) return rc;

            rc = MPI_Free_mem(reducebuf);
            if (rc != MPI_SUCCESS) return rc;
        }

        return MPI_SUCCESS;

    } else if (datatype == MPI_DOUBLE) {

        /* only used at root */
        double * gatherbuf;
        double * reducebuf;

        /* allocate buffer for summation only at root */
        if (commrank==abs(root)) {
            MPI_Aint bytes;

            bytes = (MPI_Aint)typesize * (MPI_Aint)count * (MPI_Aint)commsize;
            rc = MPI_Alloc_mem(bytes, MPI_INFO_NULL, &gatherbuf);
            if (rc != MPI_SUCCESS) return rc;

            bytes = (MPI_Aint)typesize * (MPI_Aint)count;
            rc = MPI_Alloc_mem(bytes, MPI_INFO_NULL, &reducebuf);
            if (rc != MPI_SUCCESS) return rc;
        }

        /* gather all inputs to root */
        rc = MPI_Gather(sendbuf, count, datatype, gatherbuf, count, datatype, abs(root), comm);
        if (rc != MPI_SUCCESS) return rc;

        /* perform summation only at root */
        if (commrank==abs(root)) {
            fflush(stdout);
            for (int j=0; j<count; ++j) {
                double sum = 0.0;
                double low = 0.0;
                for (int i=0; i<commsize; ++i) {
                    int index = j+i*count;
                    /* simple summation:
                     * sum += gatherbuf[index]; */
                    double y = gatherbuf[index] - low;
                    double t = sum + y;
                    low = (t - sum) - y;
                    sum = t;
                }
                reducebuf[j] = sum;
            }
            memcpy(recvbuf, reducebuf, count * sizeof(double) );
        }

        /* bcast to achieve allreduce semantic */
        if (root<0) {
            rc = MPI_Bcast(recvbuf, count, datatype, abs(root), comm);
            if (rc != MPI_SUCCESS) return rc;
        }

        /* free buffer for summation only at root */
        if (commrank==abs(root)) {
            rc = MPI_Free_mem(gatherbuf);
            if (rc != MPI_SUCCESS) return rc;

            rc = MPI_Free_mem(reducebuf);
            if (rc != MPI_SUCCESS) return rc;
        }

        return MPI_SUCCESS;

    } else if (datatype == MPI_LONG_DOUBLE) {

        /* only used at root */
        long double * gatherbuf;
        long double * reducebuf;

        /* allocate buffer for summation only at root */
        if (commrank==abs(root)) {
            MPI_Aint bytes;

            bytes = (MPI_Aint)typesize * (MPI_Aint)count * (MPI_Aint)commsize;
            rc = MPI_Alloc_mem(bytes, MPI_INFO_NULL, &gatherbuf);
            if (rc != MPI_SUCCESS) return rc;

            bytes = (MPI_Aint)typesize * (MPI_Aint)count;
            rc = MPI_Alloc_mem(bytes, MPI_INFO_NULL, &reducebuf);
            if (rc != MPI_SUCCESS) return rc;
        }

        /* gather all inputs to root */
        rc = MPI_Gather(sendbuf, count, datatype, gatherbuf, count, datatype, abs(root), comm);
        if (rc != MPI_SUCCESS) return rc;

        /* perform summation only at root */
        if (commrank==abs(root)) {
            fflush(stdout);
            for (int j=0; j<count; ++j) {
                long double sum = 0.0L;
                long double low = 0.0L;
                for (int i=0; i<commsize; ++i) {
                    int index = j+i*count;
                    /* simple summation:
                     * sum += gatherbuf[index]; */
                    long double y = gatherbuf[index] - low;
                    long double t = sum + y;
                    low = (t - sum) - y;
                    sum = t;
                }
                reducebuf[j] = sum;
            }
            //fflush(stdout);
            memcpy(recvbuf, reducebuf, count * sizeof(long double) );
        }

        /* bcast to achieve allreduce semantic */
        if (root<0) {
            rc = MPI_Bcast(recvbuf, count, datatype, abs(root), comm);
            if (rc != MPI_SUCCESS) return rc;
        }

        /* free buffer for summation only at root */
        if (commrank==abs(root)) {
            rc = MPI_Free_mem(gatherbuf);
            if (rc != MPI_SUCCESS) return rc;

            rc = MPI_Free_mem(reducebuf);
            if (rc != MPI_SUCCESS) return rc;
        }

        return MPI_SUCCESS;

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
