#include <stdio.h>

#include <mpi.h>

#include "kahanmpi.h"

#include "state.h"
#include "utils.h"

global_state_t KahanMPI_Global_State = {0};

void KahanMPI_Init(void)
{
    int rank;
    PMPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int root = 0;

    /* Parse environment variables at root and broadcast to ensure consistency.
     * If global state is not consistent across all processes, KahanMPI may deadlock. */
    if (rank==root) {
        bool active                  = KahanMPI_Getenv_bool("KAHANMPI_ACTIVE", true);
        bool use_gather_compensated  = KahanMPI_Getenv_bool("KAHANMPI_USE_GATHER_COMPENSATED", true);
        bool use_userdef_compensated = KahanMPI_Getenv_bool("KAHANMPI_USE_USERDEF_COMPENSATED", true);
        bool use_promotion           = KahanMPI_Getenv_bool("KAHANMPI_USE_PROMOTION", true);
        bool mpi_errors_are_fatal    = true; /* this is the default */
        int debug_level              = KahanMPI_Getenv_int("KAHANMPI_DEBUG_LEVEL", 1);

        KahanMPI_Global_State.active                  = active;
        KahanMPI_Global_State.use_gather_compensated  = use_gather_compensated;
        KahanMPI_Global_State.use_userdef_compensated = use_userdef_compensated;
        KahanMPI_Global_State.use_promotion           = use_promotion;
        KahanMPI_Global_State.mpi_errors_are_fatal    = mpi_errors_are_fatal;
        KahanMPI_Global_State.debug_level             = debug_level;

        int size;
        PMPI_Comm_size(MPI_COMM_WORLD, &size);

        int major, minor;
        PMPI_Get_version(&major, &minor);

        if (debug_level>0) {
            printf("KahanMPI  initialized with %d process%s, MPI v%d.%d\n", size, size > 1 ? "es":"", major, minor);
            printf("Summary of environment variables:\n");
            printf("KAHANMPI_ACTIVE                     = %s\n", active                  ? "true" : "false");
            printf("KAHANMPI_USE_GATHER_COMPENSATED     = %s\n", use_gather_compensated  ? "true" : "false");
            printf("KAHANMPI_USE_USERDEF_COMPENSATED    = %s\n", use_userdef_compensated ? "true" : "false");
            printf("KAHANMPI_USE_PROMOTION              = %s\n", use_promotion           ? "true" : "false");
            printf("KAHANMPI_DEBUG_LEVEL                = %d\n", debug_level);
        }
        fflush(stdout);
    }

    int rc = PMPI_Bcast(&KahanMPI_Global_State, sizeof(KahanMPI_Global_State),
                        MPI_BYTE, root, MPI_COMM_WORLD);
    if (rc != MPI_SUCCESS) {
        printf("%d: KahanMPI_Init: PMPI_Bcast failed\n", rank);
        PMPI_Abort(MPI_COMM_WORLD, rc);
    }
}

int MPI_Init(int * argc, char ** argv[])
{
    int rc = MPI_SUCCESS;

    rc = PMPI_Init(argc, argv);

    KahanMPI_Init();

    return rc;
}

#if defined(MPI_VERSION) && (MPI_VERSION >= 2)
int MPI_Init_thread(int * argc, char ** argv[], int required, int * provided)
{
    int rc = MPI_SUCCESS;

    rc = PMPI_Init_thread(argc, argv, required, provided);

    KahanMPI_Init();

    return rc;
}
#endif
