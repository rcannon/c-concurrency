
#include "system_includes.h"
#include "print_string.h"

#ifndef _INSERT_CLIENT_RESULT_H_
#define _INSERT_CLIENT_RESULT_H_ 1

void
insert_client_result
    ( void* client_results_base_addr
    , const int thread
    , const size_t per_client_results_shm_size
    , double* result_base_addr
    , const size_t num_elements_in_block_row_col
    , const int64_t block_row_task
    );

#endif