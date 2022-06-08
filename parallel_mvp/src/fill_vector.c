
#include "fill_vector.h"

void
fill_vector
    ( FILE* my_lfp
    , void* vector_base_addr
    , size_t num_blocks_in_matrix_row_col
    , size_t num_elements_in_block_row_col
    )
{
    double* insert_pointer;
    size_t vector_size;
    size_t iter;

    int my_thread_id;
    my_thread_id = 0; // only server should call this

    vector_size = num_blocks_in_matrix_row_col
                * num_elements_in_block_row_col;

    if ((vector_base_addr == (void *) -1) || (!vector_base_addr) ) {
        if (my_lfp){
            print_string
                ( my_lfp
                , "fill vector : addr base is -1"
                );
            fflush(my_lfp);
        }
        else { 
            print_outfile_not_found(my_thread_id);
        }
    }
    else {
        insert_pointer = vector_base_addr;

        // vector[i] = i
        for (iter = 0; iter < vector_size; iter++){
            *insert_pointer = iter;
            insert_pointer++;
        }
    }
}