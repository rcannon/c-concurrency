
#include "run_server.h"

void
run_server( FILE* my_lfp
          , void* shm_addr_base
          , int n_threads
          , size_t mem_per_thread
          , size_t num_blocks_in_matrix_row_col
          , size_t num_elements_in_block_row_col
          ) 
{
    // shared memory general
    key_t shm_key;
    int ftok_number;

    // shared memory matrix
    void* matrix_base_addr;
    int matrix_shm_id;

    // shared_memory - vector
    void* vector_base_addr;
    int matrix_shm_id;

    //shared memory - result
    void* result_base_addr;
    int result_shm_id;

    // matrix vector attributes
    size_t matrix_size;
    size_t vector_size;
    size_t result_size;

    // setup matrix shm segment
    system("touch /dev/shm/matrix");
    ftok_number = 43;
    shm_key = ftok("/dev/shm/matrix", ftok_number);
    matrix_size = pow(num_blocks_in_matrix_row_col, 2) 
                * pow(num_elements_in_block_row_col, 2)
    matrix_shm_id = shm_setup_double_array
        ( my_lfp
        , shm_key
        , matrix_size
        , &matrix_base_addr
        );
    fill_matrix
        ( my_lfp
        , &matrix_base_addr
        , num_blocks_in_matrix_row_col
        , num_elements_in_block_row_col
        );

    // setup vector shm segment
    system("touch /dev/shm/vector");
    ftok_number = 44;
    shm_key = ftok("/dev/shm/vector", ftok_number);
    vector_size = num_blocks_in_matrix_row_col 
                * num_elements_in_block_row_col
    vector_shm_id = shm_setup_double_array
        ( my_lfp
        , shm_key
        , vector_size
        , &vector_base_addr
        );
    fill_vector
        ( my_lfp
        , &vector_base_addr
        , num_blocks_in_matrix_row_col
        , num_elements_in_block_row_col
        );

    // setup result shm segmet
    system("touch /dev/shm/result");
    ftok_number = 45;
    shm_key = ftok("/dev/shm/result", ftok_number);
    result_size = num_blocks_in_matrix_row_col
                * num_elements_in_block_row_col
    result_shm_id = shm_setup_double_array
        ( my_lfp
        , shm_key
        , result_size
        , &result_base_addr
        );

    // give clients matrix vector shmids
    send_clients_mat_vec_shmids
        ( my_lfp
        , shm_addr_base
        , n_threads
        , mem_per_thread
        , matrix_shm_id
        , vector_shm_id
        );

    // get mvp result from clients
    get_mvp_result
        ( my_lfp
        , &result
        , shm_addr_base, 
        , n_threads
        , mem_per_thread
        );

    // detatch from problem specific shared memory
    shmdt(matrix_base_addr);
    shmdt(vector_base_addr);
    shmdt(result_base_addr);
}