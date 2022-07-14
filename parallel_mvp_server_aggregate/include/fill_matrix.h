
#include "system_includes.h"
#include "print_string.h"

#ifndef _FILL_MATRIX_H_
#define _FILL_MATRIX_H_ 1

void
fill_matrix
    ( FILE* my_lfp
    , void* matrix_base_addr
    , size_t num_blocks_in_matrix_row_col
    , size_t num_elements_in_block_row_col
    );

#endif
