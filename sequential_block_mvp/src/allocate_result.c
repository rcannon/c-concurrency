
#include "allocate_result.h"

void
allocate_result ( double** result
                , num_elements_in_block_row_col
                , num_blocks_in_row_col_row_col
                )
{
    int result_size;
    int save_errno;

    result_size = num_elements_in_block_row_col * num_blocks_in_row_col_row_col;
    *result = calloc(1, vector_result_size);

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