#include <mpi.h>

int MPI_Reduce_local(const void *inbuf, void *inoutbuf, int count, MPI_Datatype datatype, MPI_Op op)
{
    return MPI_Reduce_local(inbuf, inoutbuf, count, datatype, op);
}

int MPI_Reduce(const void *sendbuf, void *recvbuf, int count,
               MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm)
{
    return PMPI_Reduce(sendbuf, recvbuf, count, datatype, op, root, comm);
}

int MPI_Allreduce(const void *sendbuf, void *recvbuf, int count,
               MPI_Datatype datatype, MPI_Op op, MPI_Comm comm)
{
    return PMPI_Allreduce(sendbuf, recvbuf, count, datatype, op, comm);
}

int MPI_Reduce_scatter(const void *sendbuf, void *recvbuf, const int recvcounts[],
                       MPI_Datatype datatype, MPI_Op op, MPI_Comm comm)
{
    return PMPI_Reduce_scatter(sendbuf, recvbuf, recvcounts, datatype, op, comm);
}

int MPI_Reduce_scatter_block(const void *sendbuf, void *recvbuf, const int recvcount,
                             MPI_Datatype datatype, MPI_Op op, MPI_Comm comm)
{
    return PMPI_Reduce_scatter_block(sendbuf, recvbuf, recvcount, datatype, op, comm);
}

int MPI_Ireduce(const void *sendbuf, void *recvbuf, int count,
               MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm, MPI_Request * request)
{
    return PMPI_Ireduce(sendbuf, recvbuf, count, datatype, op, root, comm, request);
}

int MPI_Iallreduce(const void *sendbuf, void *recvbuf, int count,
               MPI_Datatype datatype, MPI_Op op, MPI_Comm comm, MPI_Request * request)
{
    return PMPI_Iallreduce(sendbuf, recvbuf, count, datatype, op, comm, request);
}

int MPI_Ireduce_scatter(const void *sendbuf, void *recvbuf, const int recvcounts[],
                       MPI_Datatype datatype, MPI_Op op, MPI_Comm comm, MPI_Request * request)
{
    return PMPI_Ireduce_scatter(sendbuf, recvbuf, recvcounts, datatype, op, comm, request);
}

int MPI_Ireduce_scatter_block(const void *sendbuf, void *recvbuf, const int recvcount,
                             MPI_Datatype datatype, MPI_Op op, MPI_Comm comm, MPI_Request * request)
{
    return PMPI_Ireduce_scatter_block(sendbuf, recvbuf, recvcount, datatype, op, comm, request);
}
