
#include "run_block_mvp.h"

void
run_block_mvp   ( double* result
                , double* matrix
                , double* vector
                , int num_blocks_in_matrix_row_col
                , int num_elements_in_block_row_col
                )
{
    double* result_position;
    double* result_block;
    double* vector_position;
    double* vector_block;
    double* matrix_position;
    double sum;

    int matrix_block_row;
    int matrix_block_col;
    int row_of_block;
    int col_of_block;
    int k;

    result_block = result;
    matrix_position = matrix;

    // need to zero out result

    // iterate over blocks, row major order
    for (matrix_block_row = 0; matrix_block_row < num_blocks_in_matrix_row_col; matrix_block_row++) {

        vector_block = vector;
        result_position = result_block;

        // initialize result to be zero
        for (k = 0; k < num_elements_in_block_row_col; k++) {
            *result_position = 0.0;
            result_position += 1;
        }

        for (matrix_block_col = 0; matrix_block_col < num_blocks_in_matrix_row_col; matrix_block_col++) {

            result_position = result_block;

            // iterate over elements in block, row major order
            for (row_of_block = 0; row_of_block < num_elements_in_block_row_col; row_of_block++) {

                vector_position = vector_block;
                sum = 0.0;

                for (col_of_block = 0; col_of_block < num_elements_in_block_row_col; col_of_block++) {

                    sum = sum + (*matrix_position) * (*vector_position);
                    vector_position += 1;
                    matrix_position += 1;
                }
                *result_position = *result_position + sum;
                result_position += 1;
            }
            // advance vector block
            vector_block = vector_position;
        }
        // advance result block
        result_block = result_position;
    }
}
