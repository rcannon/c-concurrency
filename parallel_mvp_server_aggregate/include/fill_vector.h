
#include "system_includes.h"
#include "print_string.h"

#ifndef _FILL_VECTOR_H_
#define _FILL_VECTOR_H_ 1

void
fill_vector
    ( FILE* my_lfp
    , void* vector_base_addr
    , size_t num_blocks_in_matrix_row_col
    , size_t num_elements_in_block_row_col
    );

#endif