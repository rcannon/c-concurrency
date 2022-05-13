
#include "build_vector.h"

void
build_vector( double** vector
            , int num_blocks_in_matrix_row_col
            , int num_elements_in_block_row_col
            , int debug
            )
{
    double* insert_pointer;
    int vector_size;
    int save_errno;
    int iter;

    vector_size = num_blocks_in_matrix_row_col * num_elements_in_block_row_col;

    // allocate space for vector
    *vector = calloc(1, vector_size);
    save_errno = errno;
    if(!(*vector)){
        fprintf ( stderr
                , "calloc: allocate vector failed with errno = %d, %s\n"
                , save_errno
                , strerror(save_errno)
                );
        fflush(stderr);
    }
    else {

        insert_pointer = &(**vector);

        // vector[i] = i
        for (iter = 0; iter < vector_size; iter++){
            *insert_pointer = iter;
            insert_pointer++;
        }

        if (debug && (vector_size < 10)){
            print_string(stdout,"printing vector.\n");
            for (iter = 0; iter < vector_size; iter++){
                fprintf(stdout, "%.1f ", (*vector)[iter]);
                flush(stdout);
            }
            print_string(stdout, "\n");
        }
    }
}