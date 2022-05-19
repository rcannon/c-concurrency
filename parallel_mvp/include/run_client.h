
#include "system_includes.h"
#include "print_string.h"
#include "server_struct.h"
#include "client_struct.h"
#include "print_outfile_not_found.h"

#ifndef _RUN_CLIENT_H_
#define _RUN_CLIENT_H_ 1

void 
run_client
    ( FILE* my_lfp
    , void* shm_addr_base
    , int my_thread_id
    , int n_threads
    , size_t mem_per_thread
    , size_t num_blocks_in_matrix_row_col
    , size_t num_elements_in_block_row_col
    );
          

#endif