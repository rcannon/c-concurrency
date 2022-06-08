
#include "client_mvp_block_row.h"

void
client_mvp_block_row
    ( const size_t block_row_task
    , double* matrix_addr_base
    , double* vector_addr_base
    , double* my_result_addr_base
    , const size_t num_blocks_in_matrix_row_col
    , const size_t num_elements_in_block_row_col
    )
{
    double* matrix_ptr;
    double* vector_ptr;
    double* vector_block;
    double* result_insert_ptr;
    double sum;

    size_t matrix_offset;
    size_t block;
    size_t block_row;
    size_t block_col;
    size_t i;

    matrix_offset   = block_row_task 
                    * num_blocks_in_matrix_row_col
                    * pow(num_elements_in_block_row_col,2);
    matrix_ptr      = matrix_addr_base + matrix_offset;
    vector_block    = vector_addr_base;



    // set the result vector to zeros
    result_insert_ptr = my_result_addr_base;
    for (i = 0; i < num_elements_in_block_row_col; i++){
        *result_insert_ptr = 0.0;
        result_insert_ptr++;
    }

    // do block row mvp
    for (block = 0; block < num_blocks_in_matrix_row_col; block++){

        result_insert_ptr = my_result_addr_base;

        for (block_row = 0; block_row < num_elements_in_block_row_col; block_row++){

            sum = 0.0;
            vector_ptr = vector_block;

            for (block_col = 0; block_col < num_elements_in_block_row_col; block_col++){
                sum = sum + (*matrix_ptr) * (*vector_ptr);
                matrix_ptr++;
                vector_ptr++;
            }
            *result_insert_ptr = *result_insert_ptr + sum;

            // advance result position
            result_insert_ptr++;
        }
        // advance vector position
        vector_block = vector_ptr;
    }
}