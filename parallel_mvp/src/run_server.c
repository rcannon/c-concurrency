
#include "run_server.h"

void
run_server
    ( FILE* my_lfp
    , void* shm_addr_base
    , int n_threads
    , size_t comm_mem_per_thread
    , size_t num_blocks_in_matrix_row_col
    , size_t num_elements_in_block_row_col
    ) 
{
    // shared memory general
    key_t shm_key;
    int ftok_number;

    // shared memory matrix
    void* matrix_base_addr;
    int matrix_shmid;
    size_t matrix_size;
    size_t matrix_shm_size;

    // shared_memory - vector
    void* vector_base_addr;
    int vector_shmid;
    size_t vector_size;
    size_t vector_shm_size;

    // shared memory - client_results
    void* client_results_base_addr;
    int client_results_shmid;
    size_t per_client_results_size;
    size_t per_client_results_shm_size;
    size_t total_client_results_shm_size;

    //shared memory - result
    void* result_base_addr;
    int result_shmid;
    size_t result_size;
    size_t result_shm_size;

    // check result vector
    int correct;

    // setup matrix
    system("touch /dev/shm/matrix");
    ftok_number = 43;
    shm_key = ftok("/dev/shm/matrix", ftok_number);
    matrix_size = pow(num_blocks_in_matrix_row_col, 2) 
                * pow(num_elements_in_block_row_col, 2);
    matrix_shm_size = matrix_size * sizeof(double);
    matrix_shmid = shm_setup_double_array
        ( my_lfp
        , shm_key
        , matrix_shm_size
        , &matrix_base_addr
        );
    print_string(my_lfp, "starting matrix fill");
    fflush(my_lfp);
    fill_matrix
        ( my_lfp
        , matrix_base_addr
        , num_blocks_in_matrix_row_col
        , num_elements_in_block_row_col
        );
    run_msync
        ( my_lfp
        , matrix_base_addr
        , matrix_shm_size
        );
    print_string(my_lfp, "finished matrix fill");

    // setup vector
    system("touch /dev/shm/vector");
    ftok_number = 44;
    shm_key = ftok("/dev/shm/vector", ftok_number);
    vector_size = num_blocks_in_matrix_row_col 
                * num_elements_in_block_row_col;
    vector_shm_size = vector_size * sizeof(double);
    vector_shmid = shm_setup_double_array
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
    run_msync
        ( my_lfp
        , vector_base_addr
        , vector_shm_size
        );

    // setup each client's result vector
    system("touch /dev/shm/client_result");
    ftok_number = 45;
    shm_key = ftok("/dev/shm/client_result", ftok_number);
    per_client_results_size = num_elements_in_block_row_col;
    per_client_results_shm_size = per_client_results_size * sizeof(double);
    // client result space on separate pages
    if (per_client_results_shm_size < 4096){
        per_client_results_shm_size = 4096;
    }
    else {
        per_client_results_shm_size = calc_mem_per_thread(per_client_results_shm_size);
    }
    total_client_results_shm_size = n_threads * per_client_results_shm_size;
    //fprintf(my_lfp, "client result size = %ld\n", per_client_results_shm_size);
    //fflush(my_lfp);
    client_results_shmid = shm_setup_double_array
        ( my_lfp
        , shm_key
        , total_client_results_shm_size
        , &client_results_base_addr
        );

    // give clients matrix, vector shmids
    send_shmids
        ( my_lfp
        , shm_addr_base
        , n_threads
        , comm_mem_per_thread
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
    result_shm_size = result_size * sizeof(double);
    result_shmid = shm_setup_double_array
        ( my_lfp
        , shm_key
        , result_shm_size
        , &result_base_addr
        );

    // get mvp result from clients
    mvp_server_side
        ( my_lfp
        , result_base_addr
        , num_blocks_in_matrix_row_col
        , num_elements_in_block_row_col
        , client_results_base_addr
        , n_threads
        , shm_addr_base
        , comm_mem_per_thread
        , per_client_results_shm_size
        );

    // check if result is correct
    correct = 0;
    correct = verify_result
        ( result_base_addr
        , num_blocks_in_matrix_row_col
        , num_elements_in_block_row_col
        );
    if (!correct) {
        print_string(my_lfp, "\nincorrect\n");
    }
    else {
        print_string(my_lfp, "\ncorrect\n");
    }

    // detatch from problem specific shared memory
    shmdt(matrix_base_addr);
    shmdt(vector_base_addr);
    shmdt(result_base_addr);
    shmdt(client_results_base_addr);
}