
#include "system_includes.h"
#include "print_string.h"
#include "server_struct.h"
#include "client_struct.h"

#ifndef _MVP_SERVER_SIDE_H_
#define _MVP_SERVER_SIDE_H_ 1

void
mvp_server_side
    ( FILE* my_lfp
    , void* result_base_addr
    , const size_t num_blocks_in_matrix_row_col
    , const size_t num_elements_in_block_row_col
    , void* client_results_base_addr
    , const int n_threads
    , void* comm_shm_addr_base
    , const size_t comm_mem_per_thread
    , const size_t per_client_results_shm_size
    );

#endif