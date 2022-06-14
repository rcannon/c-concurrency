
#include "system_includes.h"

#ifndef _RUN_MSYNC_H_
#define _RUN_MSYNC_H_ 1

void
run_msync
    ( FILE* my_lfp
    , void* array_addr_base
    , size_t msync_size
    );

#endif