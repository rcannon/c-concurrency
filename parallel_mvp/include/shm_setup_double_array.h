
#include "system_includes.h"
#include "print_outfile_not_found.h"
#include "attach_shm.h"

#ifndef _SHM_SETUP_DOUBLE_ARRAY_H_
#define _SHM_SETUP_DOUBLE_ARRAY_H_ 1

int
shm_setup_double_array
    ( FILE* my_lfp
    , key_t shm_key
    , size_t shm_size
    , void** shm_addr_base
    );

#endif