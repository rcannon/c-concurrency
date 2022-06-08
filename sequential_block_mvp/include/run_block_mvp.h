
#include "system_includes.h"
#include "print_string.h"

#ifndef _RUN_BLOCK_MVP_H_
#define _RUN_BLOCK_MVP_H_ 1

void
run_block_mvp   ( double* result
                , double* matrix
                , double* vector
                , int num_blocks_in_matrix_row_col
                , int num_elements_in_block_row_col
                );

#endif