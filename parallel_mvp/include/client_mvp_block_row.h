
#include "system_includes.h"

#ifndef _CLIENT_MVP_BLOCK_ROW_H_
#define _CLIENT_MVP_BLOCK_ROW_H_ 1

void
client_mvp_block_row
    ( const size_t block_row_task
    , double* matrix_addr_base
    , double* vector_addr_base
    , double* my_result_addr_base
    , const size_t num_blocks_in_matrix_row_col
    , const size_t num_elements_in_block_row_col
    );

#endif