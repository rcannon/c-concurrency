
#ifndef _SHM_CLIENT_H_
#define _SHM_CLIENT_H_ 1

struct client_struct
{
    int64_t dialog_counter;
    int64_t matrix_shm_id;
    double result;
    size_t vector_result_index;
};

#endif