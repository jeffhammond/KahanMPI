#include "mybool.h"

typedef struct {
    bool use_gather_compensated;
    bool use_promotion;
} global_state_t;

extern global_state_t KahanMPI_Global_State;
