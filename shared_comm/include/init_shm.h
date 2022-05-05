
#include "system_includes.h"
#include "print_outfile_not_found.h"

#ifndef _INIT_SHM_H_
#define _INIT_SHM_H_ 1

void
init_shm( int n_threads
        , size_t mem_per_thread
        , void** shm_addr_base
        );

#endif