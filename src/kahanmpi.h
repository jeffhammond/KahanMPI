#include <stdbool.h>

#include <mpi.h>

static inline bool kahanmpi_type_op_supported(MPI_Datatype type, MPI_Op op)
{
    if (op == MPI_SUM) {
        if (type == MPI_FLOAT) return true;
    }
    return false;
}

int KahanMPI_Reduce_local(const void *inbuf, void *inoutbuf, int count,
                          MPI_Datatype datatype, MPI_Op op);
int KahanMPI_Reduce(const void *sendbuf, void *recvbuf, int count,
                    MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm);
int KahanMPI_Allreduce(const void *sendbuf, void *recvbuf, int count,
                       MPI_Datatype datatype, MPI_Op op, MPI_Comm comm);
int KahanMPI_Reduce_scatter(const void *sendbuf, void *recvbuf, const int recvcounts[],
                            MPI_Datatype datatype, MPI_Op op, MPI_Comm comm);
int KahanMPI_Reduce_scatter_block(const void *sendbuf, void *recvbuf, const int recvcount,
                                  MPI_Datatype datatype, MPI_Op op, MPI_Comm comm);
int KahanMPI_Ireduce(const void *sendbuf, void *recvbuf, int count,
                     MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm, MPI_Request * request);
int KahanMPI_Iallreduce(const void *sendbuf, void *recvbuf, int count,
                        MPI_Datatype datatype, MPI_Op op, MPI_Comm comm, MPI_Request * request);
int KahanMPI_Ireduce_scatter(const void *sendbuf, void *recvbuf, const int recvcounts[],
                             MPI_Datatype datatype, MPI_Op op, MPI_Comm comm, MPI_Request * request);
int KahanMPI_Ireduce_scatter_block(const void *sendbuf, void *recvbuf, const int recvcount,
                                   MPI_Datatype datatype, MPI_Op op, MPI_Comm comm, MPI_Request * request);
