
#include "system_includes.h"
#include "print_string.h"

#ifndef _INSERT_CLIENT_RESULT_H_
#define _INSERT_CLIENT_RESULT_H_ 1

void
insert_client_result
    ( double* client_result_area
    , double* result_base_addr
    , const size_t num_elements_in_block_row_col
    , const int64_t block_row_task
    );

#endif