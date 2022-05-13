
#include "run_block_mvp.h"

void
run_block_mvp   ( double** result
                , double** matrix
                , double** vector
                , int num_blocks_in_matrix_row_col
                , int num_elements_in_block_row_col
                )
{
    double* insert_pointer;
    double* vector_access_pointer;
    double* matrix_access_pointer;

    int matrix_block_row;
    int matrix_block_col;
    int row_of_block;
    int col_of_block;

    insert_pointer = &(**result);
    vector_access_pointer = &(**vector);
    matrix_access_pointer = &(**matrix);

    // iterate over blocks, row major order
    for (matrix_block_row = 0; matrix_block_row < num_blocks_in_matrix_row_col; matrix_block_row++) {
        for (matrix_block_col = 0;matrix_block_col < num_blocks_in_matrix_row_col; matrix_block_col++) {

            // iterate over elements in block, row major order
            for (row_of_block = 0; row_of_block < num_elements_in_block_row_col; row_of_block++) {
                for (col_of_block= 0; col_of_block < num_elements_in_block_row_col; col_of_block++) {

                    *insert_pointer += (*vector_access_pointer) * (*matrix_access_pointer);
                    vector_access_pointer++;
                    matrix_access_pointer++;
                }
                insert_pointer++;
                vector_access_pointer -= num_elements_in_block_row_col;
            }
            vector_access_pointer += num_elements_in_block_row_col;
            insert_pointer -= num_blocks_in_matrix_row_col;
        }
        vector_access_pointer = &(*vector);
    }
}

/*
reg matrix
 4  1  1  1
-1  4  1  1
-1 -1  4  1
-1 -1 -1  4

block
 4  1 -1  4     0
 1  1  1  1     1
-1 -1 -1 -1     2
 4  1 -1  4     3

result


*/