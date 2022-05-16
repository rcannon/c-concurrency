
#include "allocate_result.h"

void
allocate_result ( double** result
                , int num_blocks_in_matrix_row_col
                , int num_elements_in_block_row_col
                )
{
    size_t result_size;
    size_t alloc_size;
    int save_errno;

    result_size = num_elements_in_block_row_col * num_blocks_in_matrix_row_col;
    alloc_size = result_size * sizeof(double);
    *result = calloc(1, alloc_size);

    save_errno = errno;
    if (!(*result)) {
        fprintf ( stderr
                , "calloc: allocate result failed with errno = %d, %s\n"
                , save_errno
                , strerror(save_errno)
                );
        fflush(stderr);
    }
}