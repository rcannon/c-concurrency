
#include "fill_matrix.h"

void
fill_matrix
    ( FILE* my_lfp
    , void* matrix_base_addr
    , size_t num_blocks_in_matrix_row_col
    , size_t num_elements_in_block_row_col
    )
{

    double* insert_pointer;

    size_t num_rows_cols_in_matrix;
    size_t num_elements_in_block;

    size_t matrix_row_block_iter;
    size_t matrix_col_block_iter;
    size_t block_row_iter;
    size_t block_col_iter;
    size_t element_iter;

    int my_thread_id;
    my_thread_id = 0; // only server should call this

    num_row_cols_in_matrix  = num_blocks_in_matrix_row_col 
                            *  num_elements_in_block_row_col;
    num_elements_in_block = pow(num_elements_in_block_row_col, 2);

    if ((matrix_base_addr == (void *) -1) || (!matrix_base_addr) ) {
        if (my_lfp){
            print_string
                ( my_lfp
                , "communication shm_addr_base is -1"
                );
            fflush(my_lfp);
        }
        else { 
            print_outfile_not_found(my_thread_id);
        }
    }
    else {
        insert_pointer = matrix;

        // build the matrix - block row major order

        /*
        Rules for our Desired Matrix:
        - diagonal elements should be the number of rows/cols
        - elements above the diagonal should be 1
        - elements below the diagonal should be -1
        */

        for ( matrix_row_block_iter = 0; matrix_row_block_iter < num_blocks_in_matrix_row_col; matrix_row_block_iter++){
            for (matrix_col_block_iter = 0; matrix_col_block_iter < num_blocks_in_matrix_row_col; matrix_col_block_iter++){

                if (matrix_row_block_iter == matrix_col_block_iter){
                    // on diagonal block
                    for (block_row_iter = 0; block_row_iter < num_elements_in_block_row_col; block_row_iter++){
                        for (block_col_iter = 0; block_col_iter < num_elements_in_block_row_col; block_col_iter++){

                            if (block_row_iter == block_col_iter){
                                // on diagonal element of block
                                *insert_pointer = (double) num_rows_cols_in_matrix;
                            }
                            else if (block_row_iter < block_col_iter){
                                // above diagonal of block
                                *insert_pointer = 1.0;
                            }
                            else {
                                // below diagonal of block
                                *insert_pointer = -1.0;
                            }
                        insert_pointer++;
                        }
                    }
                }
                else if (matrix_row_block_iter < matrix_col_block_iter){
                    // block is above diagonal
                    for (element_iter = 0; element_iter < num_elements_in_block; element_iter++){
                        *insert_pointer = 1;
                        insert_pointer++;
                    }
                }
                else{
                    // block is below diagonal
                    for (element_iter = 0; element_iter < num_elements_in_block; element_iter++){
                        *insert_pointer = -1;
                        insert_pointer++;
                    }
                }
            }
        }
    }
}