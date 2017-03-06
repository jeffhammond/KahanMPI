#include <mpi.h>

#include "utils.h"
#include "state.h"

int MPI_Init(int * argc, char ** argv[])
{
    int rc = MPI_SUCCESS;

    rc = PMPI_Init(argc, argv);

    return rc;
}

#if defined(MPI_VERSION) && (MPI_VERSION >= 2)
int MPI_Init_thread(int * argc, char ** argv[], int required, int * provided)
{
    int rc = MPI_SUCCESS;

    rc = PMPI_Init_thread(argc, argv, required, provided);

    return rc;
}
#endif
