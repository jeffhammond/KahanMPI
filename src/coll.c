#include <mpi.h>

#include "kahanmpi.h"

#include "state.h"
#include "debug.h"

int MPI_Reduce(const void *sendbuf, void *recvbuf, int count,
               MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm)
{
    if (KahanMPI_Global_State.active) {
        KahanMPI_Warning("KahanMPI is active\n");
#ifndef DISABLE_USE_GATHER_COMPENSATED
        if (KahanMPI_Global_State.use_gather_compensated) {
            KahanMPI_Warning("KahanMPI_Reduce_gather_compensated\n");
            int rc = KahanMPI_Reduce_gather_compensated(sendbuf, recvbuf, count, datatype, op, root, comm);
            if (rc == MPI_SUCCESS) return rc;
        }
#endif
#ifndef DISABLE_USE_USERDEF_COMPENSATED
        if (KahanMPI_Global_State.use_userdef_compensated) {
            KahanMPI_Warning("KahanMPI_Reduce_userdef_compensated\n");
            int rc = KahanMPI_Reduce_userdef_compensated(sendbuf, recvbuf, count, datatype, op, root, comm);
            if (rc == MPI_SUCCESS) return rc;
        }
#endif
#ifndef DISABLE_USE_PROMOTION
        if (KahanMPI_Global_State.use_promotion) {
            KahanMPI_Warning("KahanMPI_Reduce_promote\n");
            int rc = KahanMPI_Reduce_promote(sendbuf, recvbuf, count, datatype, op, root, comm);
            if (rc == MPI_SUCCESS) return rc;
        }
#endif
        KahanMPI_Error("KahanMPI: no implementation succeeded!\n");
    }
    return PMPI_Reduce(sendbuf, recvbuf, count, datatype, op, root, comm);
}

int MPI_Allreduce(const void *sendbuf, void *recvbuf, int count,
               MPI_Datatype datatype, MPI_Op op, MPI_Comm comm)
{
    return PMPI_Allreduce(sendbuf, recvbuf, count, datatype, op, comm);
}

#if 0
int MPI_Reduce_local(const void *inbuf, void *inoutbuf, int count, MPI_Datatype datatype, MPI_Op op)
{
    if (kahanmpi_type_op_supported(datatype, op)) {
        return KahanMPI_Reduce_local(inbuf, inoutbuf, count, datatype, op);
    }
    return PMPI_Reduce_local(inbuf, inoutbuf, count, datatype, op);
}

int MPI_Reduce_scatter(const void *sendbuf, void *recvbuf, const int recvcounts[],
                       MPI_Datatype datatype, MPI_Op op, MPI_Comm comm)
{
    if (kahanmpi_type_op_supported(datatype, op)) {
        return KahanMPI_Reduce_scatter(sendbuf, recvbuf, recvcounts, datatype, op, comm);
    }
    return PMPI_Reduce_scatter(sendbuf, recvbuf, recvcounts, datatype, op, comm);
}

int MPI_Reduce_scatter_block(const void *sendbuf, void *recvbuf, const int recvcount,
                             MPI_Datatype datatype, MPI_Op op, MPI_Comm comm)
{
    if (kahanmpi_type_op_supported(datatype, op)) {
        return KahanMPI_Reduce_scatter_block(sendbuf, recvbuf, recvcount, datatype, op, comm);
    }
    return PMPI_Reduce_scatter_block(sendbuf, recvbuf, recvcount, datatype, op, comm);
}

int MPI_Ireduce(const void *sendbuf, void *recvbuf, int count,
               MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm, MPI_Request * request)
{
    if (kahanmpi_type_op_supported(datatype, op)) {
        return KahanMPI_Ireduce(sendbuf, recvbuf, count, datatype, op, root, comm, request);
    }
    return PMPI_Ireduce(sendbuf, recvbuf, count, datatype, op, root, comm, request);
}

int MPI_Iallreduce(const void *sendbuf, void *recvbuf, int count,
               MPI_Datatype datatype, MPI_Op op, MPI_Comm comm, MPI_Request * request)
{
    if (kahanmpi_type_op_supported(datatype, op)) {
        return KahanMPI_Iallreduce(sendbuf, recvbuf, count, datatype, op, comm, request);
    }
    return PMPI_Iallreduce(sendbuf, recvbuf, count, datatype, op, comm, request);
}

int MPI_Ireduce_scatter(const void *sendbuf, void *recvbuf, const int recvcounts[],
                       MPI_Datatype datatype, MPI_Op op, MPI_Comm comm, MPI_Request * request)
{
    if (kahanmpi_type_op_supported(datatype, op)) {
        return KahanMPI_Ireduce_scatter(sendbuf, recvbuf, recvcounts, datatype, op, comm, request);
    }
    return PMPI_Ireduce_scatter(sendbuf, recvbuf, recvcounts, datatype, op, comm, request);
}

int MPI_Ireduce_scatter_block(const void *sendbuf, void *recvbuf, const int recvcount,
                             MPI_Datatype datatype, MPI_Op op, MPI_Comm comm, MPI_Request * request)
{
    if (kahanmpi_type_op_supported(datatype, op)) {
        return KahanMPI_Ireduce_scatter_block(sendbuf, recvbuf, recvcount, datatype, op, comm, request);
    }
    return PMPI_Ireduce_scatter_block(sendbuf, recvbuf, recvcount, datatype, op, comm, request);
}
#endif
