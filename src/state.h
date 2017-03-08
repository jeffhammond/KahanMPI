#ifndef HAVE_STATE_H
#define HAVE_STATE_H

#include "mybool.h"

typedef struct {
    bool active;
    bool use_gather_compensated;
    bool use_userdef_compensated;
    bool use_promotion;
    bool mpi_errors_are_fatal;
    int  debug_level;
    int  mpi_world_rank;
} global_state_t;

extern global_state_t KahanMPI_Global_State;

#endif // HAVE_STATE_H
