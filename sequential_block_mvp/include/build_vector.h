
#include "system_includes.h"
#include "print_string.h"

#ifndef _BUILD_VECTOR_H_
#define _BUILD_VECTOR_H_ 1

void
build_vector( double** vector
            , int num_blocks_in_matrix_row_col
            , int num_elements_in_block_row_col
            , int debug
            );

#endif