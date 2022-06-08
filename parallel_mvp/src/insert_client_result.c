
#include "insert_client_result.h"

void
insert_client_result
    ( void* client_results_base_addr
    , const int thread
    , const size_t per_client_results_shm_size
    , double* result_base_addr
    , const size_t num_elements_in_block_row_col
    , const int64_t block_row_task
    )
{
    double* result_insert_ptr;
    double* client_result_area;

    int i;

    // caution address arithmetic
    client_result_area  = client_results_base_addr                  
                        + (thread * per_client_results_shm_size);   // size of double incorperated into shm_size
    result_insert_ptr   = result_base_addr                                  
                        + (block_row_task * num_elements_in_block_row_col);

    for (i = 0; i <  num_elements_in_block_row_col; i++){
        *result_insert_ptr = *client_result_area;
        result_insert_ptr++;
        client_result_area++;
    }
}
