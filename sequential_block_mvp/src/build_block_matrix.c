
#include "build_block_matrix.h"

void
build_block_matrix  ( double** matrix
                    , int num_blocks_in_matrix_row_col
                    , int num_elements_in_block_row_col
                    , int debug
                    )
{
    double* insert_pointer;
    size_t alloc_size;

    int num_blocks_in_matrix
    int num_elements_in_block;
    int num_rows_colsin_matrix;
    int num_elements_in_matrix;

    int save_errno;

    int matrix_row_block_iter;
    int matrix_col_block_iter;
    int block_row_iter;
    int block_col_iter;
    int element_iter;
    
    // assuming everythins is square
    num_rows_cols_in_matrix = num_blocks_in_matrix_row_col * num_elements_in_block_row_col;
    num_blocks_in_matrix = pow(num_blocks_in_matrix_row_col,2);
    num_elements_in_block = pow(num_elements_in_block_row_col, 2);
    num_elements_in_matrix = num_blocks_in_matrix * num_elements_in_block;

    // allocate space for the matrix
    alloc_size = num_elements_in_matrix * sizeof(double);
    *matrix = (double*) calloc(1, alloc_size);
    save_errno = errno;

    if (!(*matrix)) {
        fprintf ( stderr
                , "calloc: allocate matrix failed with errno = %d, %s\n"
                , save_errno
                , strerror(save_errno)
                );
        fflush(stderr);
    }
    else {
        insert_pointer = &(*matrix);

        // build the matrix - block row major order

        /*
        Rules for our Desired Matrix:
        - diagonal elements should be the number of rows/cols
        - elements above the diagonal should be 1
        - elements below the diagonal should be -1
        */

        for ( matrix_row_block_iter = 0; matrix_row_block_iter < num_blocks_in_matrix_row_col, matrix_row_block_iter++){
            for (matrix_col_block_iter = 0; matrix_col_block_iter < num_blocks_in_matrix_row_col, matrix_col_block_iter++){

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
                    for (element_iter = 0; element_iter < num_elements_in_block, element_iter++){
                        *insert_pointer = 1;
                        insert_pointer++;
                    }
                }
                else{
                    // block is below diagonal
                    for (element_iter = 0; element_iter < num_elements_in_block, element_iter++){
                        *insert_pointer = -1;
                        insert_pointer++;
                    }
                }
            }
        }
        if (debug && (num_elements_in_matrix < 17)){
            print_string("printing block matrix.\n");
            for (element_iter = 0; element_iter < num_elements_in_matrix; element_iter++){
                if (element_iter % num_elements_in_block == 0 ){
                    print_string(stdout, "\n");
                }
                fprintf(stdout, "%.1d ", (*matrix)[element_iter]);
                flush(stdout);
            }
            print_string("\n");
        }
    }
}