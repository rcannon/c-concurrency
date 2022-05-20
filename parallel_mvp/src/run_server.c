
#include "run_server.h"

void
run_server
    ( FILE* my_lfp
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
    size_t matrix_size;
    size_t matrix_shm_size;

    // shared_memory - vector
    void* vector_base_addr;
    int vector_shmid;
    size_t vector_size;
    size_t vector_shm_size;

    // shared memory - client_results
    void* client_results_addr_base;
    int client_results_shmid;
    size_t client_results_size;
    size_t client_results_shm_size;

    //shared memory - result
    void* result_base_addr;
    int result_shmid;
    size_t result_size;
    size_t result_size;

    // setup matrix
    system("touch /dev/shm/matrix");
    ftok_number = 43;
    shm_key = ftok("/dev/shm/matrix", ftok_number);
    matrix_size = pow(num_blocks_in_matrix_row_col, 2) 
                * pow(num_elements_in_block_row_col, 2);
    matrix_shm_size = calc_shm_size(matrix_size * sizeof(double));
    matrix_shm_id = shm_setup_double_array
        ( my_lfp
        , shm_key
        , matrix_shm_size
        , &matrix_base_addr
        );
    fill_matrix
        ( my_lfp
        , matrix_base_addr
        , num_blocks_in_matrix_row_col
        , num_elements_in_block_row_col
        );

    // setup vector
    system("touch /dev/shm/vector");
    ftok_number = 44;
    shm_key = ftok("/dev/shm/vector", ftok_number);
    vector_size = num_blocks_in_matrix_row_col 
                * num_elements_in_block_row_col;
    vector_shm_size = calc_shm_size(vector_size * sizeof(double));
    vector_shm_id = shm_setup_double_array
        ( my_lfp
        , shm_key
        , vector_shm_size
        , &vector_base_addr
        );
    fill_vector
        ( my_lfp
        , vector_base_addr
        , num_blocks_in_matrix_row_col
        , num_elements_in_block_row_col
        );

    // setup each client's result vector
    system("touch /dev/shm/client_result");
    ftok_number = 45;
    shm_key = ftok("/dev/shm/client_result", ftok_number);
    client_results_size 
        = n_threads
        * num_elements_in_block_row_col;
    client_results_shm_size = calc_shm_size(client_results_size * sizeof(double));
    client_results_shmid = shm_setup_double_array
        ( my_lfp
        , shm_key
        , client_results_shm_size
        , &client_results_base_addr
        );


    // give clients matrix, vector shmids
    send_mat_vec_res_shmids
        ( my_lfp
        , shm_addr_base
        , n_threads
        , mem_per_thread
        , matrix_shmid
        , vector_shmid
        , client_results_shmid
        );

    // setup overall result vector
    system("touch /dev/shm/result");
    ftok_number = 46;
    shm_key = ftok("/dev/shm/result", ftok_number);
    result_size = num_blocks_in_matrix_row_col
                * num_elements_in_block_row_col;
    results_shm_size = calc_shm_size(results_size * sizeof(double));
    result_shm_id = shm_setup_double_array
        ( my_lfp
        , shm_key
        , results_shm_size
        , &result_base_addr
        );

    // get mvp result from clients
    get_mvp_result
        ( my_lfp
        , result
        , shm_addr_base, 
        , n_threads
        , mem_per_thread
        );

    // detatch from problem specific shared memory
    shmdt(matrix_base_addr);
    shmdt(vector_base_addr);
    shmdt(result_base_addr);
}