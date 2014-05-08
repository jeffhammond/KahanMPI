#ifdef DEBUG
#  include <cstdio>
#endif

#include <cstdlib>  /* declares NULL    */
#include <cmath>    /* declares ldexp() */
#include <new>      /* declares new[]   */
#include <mpi.h>

#define MPIX_KAHAN_SUM (MPI_Op)0x99999999

#ifdef __cplusplus
extern "C" {
#endif

int My_Allreduce_kahan(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm )
{
    int rc = MPI_SUCCESS;

#ifdef DEBUG
    fprintf(stderr, "Using deterministic MPI_Allreduce \n");
#endif

    int commrank = 0;
    rc = MPI_Comm_rank(comm, &commrank);
    if (rc!=MPI_SUCCESS) return rc;

    int commsize = 0;
    rc = MPI_Comm_size(comm, &commsize);
    if (rc!=MPI_SUCCESS) return rc;

    int typesize = 0;
    rc = MPI_Type_size(datatype, &typesize);
    if (rc!=MPI_SUCCESS) return rc;

    void * tempbuf = NULL;
    rc = MPI_Alloc_mem( (MPI_Aint)(commsize*count*typesize), MPI_INFO_NULL, &tempbuf);
    if (rc!=MPI_SUCCESS) return rc;

#ifdef USE_LOG_N_ALGORITHM
    int root = 0;
    rc = MPI_Gather(sendbuf, count, datatype, tempbuf, count, datatype, root, comm);
#else
    rc = MPI_Allgather(sendbuf, count, datatype, tempbuf, count, datatype, comm);
#endif
    if (rc!=MPI_SUCCESS) return rc;

#ifdef USE_LOG_N_ALGORITHM
    if (commrank==root)
#endif
    {
        /* reduce tempbuf into recvbuf */
#if defined(USE_REDUCE_LOCAL)
        for (int i=0; i<commsize; ++i) {
            rc = MPI_Reduce_local( &(tempbuf[i*count]), recvbuf, count, datatype, op);
            if (rc!=MPI_SUCCESS) return rc;
        }
#elif defined(USE_KAHAN_SUM)
        if (datatype==MPI_DOUBLE)
            //KahanWrapper<double>(commsize, count, (double*)tempbuf, (double*)recvbuf);
            KahanWrapper<double>(commsize, count, static_cast<double*>(tempbuf), static_cast<double*>(recvbuf) );
        else {
            MPI_Abort(MPI_COMM_WORLD, 1);
            printf("KahanWrapper not instantiated for this type! \n");
        }
#else
#  error You need to define either USE_REDUCE_LOCAL or USE_KAHAN_SUM!
#endif
    }

#ifdef USE_KAHAN_SUM
#endif

#ifdef USE_LOG_N_ALGORITHM
    rc = MPI_Bcast(recvbuf, count, datatype, root, comm);
    if (rc!=MPI_SUCCESS) return rc;
#endif

    return MPI_SUCCESS;
}

/* MPI 3.0 added const to input i.e. read-only arguments... */
#if MPI_VERSION >= 3
int MPI_Allreduce(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm )
#else
int MPI_Allreduce(void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm )
#endif
{
    int rc = MPI_SUCCESS;

    if (op==MPIX_KAHAN_SUM)
        rc = My_Allreduce_kahan(sendbuf, recvbuf, count, datatype, op, comm); 
    else
        rc = PMPI_Allreduce(sendbuf, recvbuf, count, datatype, op, comm);
    if (rc!=MPI_SUCCESS) return rc;

    return MPI_SUCCESS;
}

#ifdef __cplusplus
}
#endif
