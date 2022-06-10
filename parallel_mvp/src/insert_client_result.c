
#include "insert_client_result.h"

void
insert_client_result
    ( double* client_result_area
    , double* result_base_addr
    , const size_t num_elements_in_block_row_col
    , const int64_t block_row_task
    )
{
    double* result_insert_ptr;
    int i;

    // caution address arithmetic
    result_insert_ptr   = result_base_addr                                  
                        + (block_row_task * num_elements_in_block_row_col);

    for (i = 0; i <  num_elements_in_block_row_col; i++){
        *result_insert_ptr = *client_result_area;
        result_insert_ptr++;
        client_result_area++;
    }
}
