#include "mybool.h"

#include <mpi.h>

static inline bool kahanmpi_type_op_supported(MPI_Datatype type, MPI_Op op)
{
    if (op == MPI_SUM) {
        if (type == MPI_FLOAT) return true;
        if (type == MPI_DOUBLE) return true;
        if (type == MPI_LONG_DOUBLE) return true;
    }
    return false;
}

void KahanMPI_Init(void);

int KahanMPI_Reduce_gather_compensated(const void *sendbuf, void *recvbuf, int count,
                    MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm);
#if 0
int KahanMPI_Allreduce_gather_compensated(const void *sendbuf, void *recvbuf, int count,
                       MPI_Datatype datatype, MPI_Op op, MPI_Comm comm);
int KahanMPI_Reduce_scatter_gather_compensated(const void *sendbuf, void *recvbuf, const int recvcounts[],
                            MPI_Datatype datatype, MPI_Op op, MPI_Comm comm);
int KahanMPI_Reduce_scatter_block_gather_compensated(const void *sendbuf, void *recvbuf, const int recvcount,
                                  MPI_Datatype datatype, MPI_Op op, MPI_Comm comm);
int KahanMPI_Ireduce_gather_compensated(const void *sendbuf, void *recvbuf, int count,
                     MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm, MPI_Request * request);
int KahanMPI_Iallreduce_gather_compensated(const void *sendbuf, void *recvbuf, int count,
                        MPI_Datatype datatype, MPI_Op op, MPI_Comm comm, MPI_Request * request);
int KahanMPI_Ireduce_scatter_gather_compensated(const void *sendbuf, void *recvbuf, const int recvcounts[],
                             MPI_Datatype datatype, MPI_Op op, MPI_Comm comm, MPI_Request * request);
int KahanMPI_Ireduce_scatter_block_gather_compensated(const void *sendbuf, void *recvbuf, const int recvcount,
                                   MPI_Datatype datatype, MPI_Op op, MPI_Comm comm, MPI_Request * request);
int KahanMPI_Reduce_local_gather_compensated(const void *inbuf, void *inoutbuf, int count,
                          MPI_Datatype datatype, MPI_Op op);
#endif

int KahanMPI_Reduce_userdef_compensated(const void *sendbuf, void *recvbuf, int count,
                    MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm);
#if 0
int KahanMPI_Allreduce_userdef_compensated(const void *sendbuf, void *recvbuf, int count,
                    MPI_Datatype datatype, MPI_Op op, MPI_Comm comm);
int KahanMPI_Reduce_scatter_userdef_compensated(const void *sendbuf, void *recvbuf, const int recvcounts[],
                    MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm);
int KahanMPI_Reduce_scatter_block_userdef_compensated(const void *sendbuf, void *recvbuf, const int recvcount,
                    MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm);
int KahanMPI_Ireduce_userdef_compensated(const void *sendbuf, void *recvbuf, int count,
                    MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm, MPI_Request * request);
int KahanMPI_Iallreduce_userdef_compensated(const void *sendbuf, void *recvbuf, int count,
                    MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm, MPI_Request * request);
int KahanMPI_Ireduce_scatter_userdef_compensated(const void *sendbuf, void *recvbuf, const int recvcounts[],
                    MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm, MPI_Request * request);
int KahanMPI_Ireduce_scatter_block_userdef_compensated(const void *sendbuf, void *recvbuf, const int recvcount,
                    MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm, MPI_Request * request);
int KahanMPI_Reduce_local_userdef_compensated(const void *inbuf, void *inoutbuf, int count, MPI_Datatype datatype, MPI_Op op)
#endif

int KahanMPI_Reduce_promote(const void *sendbuf, void *recvbuf, int count,
                    MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm);
#if 0
int KahanMPI_Allreduce_promote(const void *sendbuf, void *recvbuf, int count,
                    MPI_Datatype datatype, MPI_Op op, MPI_Comm comm);
int KahanMPI_Reduce_scatter_promote(const void *sendbuf, void *recvbuf, const int recvcounts[],
                    MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm);
int KahanMPI_Reduce_scatter_block_promote(const void *sendbuf, void *recvbuf, const int recvcount,
                    MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm);
int KahanMPI_Ireduce_promote(const void *sendbuf, void *recvbuf, int count,
                    MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm, MPI_Request * request);
int KahanMPI_Iallreduce_promote(const void *sendbuf, void *recvbuf, int count,
                    MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm, MPI_Request * request);
int KahanMPI_Ireduce_scatter_promote(const void *sendbuf, void *recvbuf, const int recvcounts[],
                    MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm, MPI_Request * request);
int KahanMPI_Ireduce_scatter_block_promote(const void *sendbuf, void *recvbuf, const int recvcount,
                    MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm, MPI_Request * request);
int KahanMPI_Reduce_local_promote(const void *inbuf, void *inoutbuf, int count, MPI_Datatype datatype, MPI_Op op);
#endif

