#include "kahanmpiconf.h"

#if HAVE_STDBOOL_H
#include <stdbool.h>
#else
typedef int bool;
const bool false=0;
const bool true=1;
#endif
