
#include "verify_result.h"

int
verify_result   ( double *result
                , int num_blocks_in_matrix_row_col
                , int num_elements_in_block_row_col
                )
{
    double* result_iter;
    size_t result_size;
    int correct;
    size_t i;
    double answer;

    correct = 1;
    result_size = num_blocks_in_matrix_row_col * num_elements_in_block_row_col;
    result_iter = result;

    for (i = 0; i < result_size; i++){
        answer = (double) result_size * (result_size-1)/2 + i * (result_size - i);
        correct = correct && (*result_iter == answer);
        result_iter++;
    }
    return correct;
}