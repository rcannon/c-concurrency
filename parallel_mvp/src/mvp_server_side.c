
#include "mvp_server_side.h"

void
mvp_server_side
    ( FILE* my_lfp
    , void* result_base_addr
    , const size_t num_blocks_in_matrix_row_col
    , const size_t num_elements_in_block_row_col
    , void* client_results_base_addr
    , const int n_threads
    , void* comm_shm_addr_base
    , const size_t comm_mem_per_thread
    , const size_t per_client_results_shm_size
    )
{
    void* client_shm_area;
    void* client_client_area;
    void* client_server_area;
    void* client_result_area;

    double* result_insert_ptr;
    volatile struct client_struct * client_client_data;
    struct server_struct * client_server_data;
    size_t client_server_size;
    size_t result_size;
    size_t client_results_size_per_thread;
    size_t next_block_row_task;
    size_t num_finished_block_row_tasks;
    int thread;
    int ret_val;
    int save_errno;
    int i;

    client_server_size = sizeof(struct server_struct);
    result_size = num_blocks_in_matrix_row_col * num_elements_in_block_row_col;
    client_results_size_per_thread = num_elements_in_block_row_col;
    next_block_row_task = 0;

    // send initial tasks to clients
    while (next_block_row_task < n_threads-1){

        for (thread = 1; thread < n_threads; thread++){

            client_shm_area = comm_shm_addr_base + (thread * comm_mem_per_thread);
            client_server_area = client_shm_area;
            client_client_area = client_server_area + (comm_mem_per_thread / 2);
            client_server_data = (struct server_struct *) client_server_area;
            client_client_data = (struct client_struct *) client_client_area;

            if (client_client_data->dialog_counter > client_server_data->dialog_counter) {
                
                client_server_data->block_row_task = next_block_row_task;
                ret_val = msync ( client_server_area, client_server_size, MS_SYNC );
                save_errno = errno;
                if (ret_val != 0) { // error occured 
                    if (my_lfp){
                        fprintf 
                            ( my_lfp
                            , "msync initial jobs: syncing failed with errno = %d, %s\n"
                            , save_errno
                            , strerror(save_errno) 
                            );
                        fflush(my_lfp);
                    }
                }
                
                // increase dialog counter: client is ready to work on problem
                client_server_data->dialog_counter +=1;
                ret_val = msync ( client_server_area, client_server_size, MS_SYNC );
                save_errno = errno;
                if (ret_val != 0){ // error occured
                    if (my_lfp){
                        fprintf 
                            ( my_lfp
                            , "msync initial jobs dialog counter: syncing failed with errno = %d, %s\n"
                            , save_errno
                            , strerror(save_errno)
                            );
                        fflush(my_lfp);
                    }
                }
                next_block_row_task += 1;
            }
        }
    }
    // server loop: read results and hand out next task
    num_finished_block_row_tasks = 0;
    while (num_finished_block_row_tasks < num_blocks_in_matrix_row_col){

        for (thread = 1; thread < n_threads; thread++){

            client_shm_area = comm_shm_addr_base + (thread * comm_mem_per_thread);
            client_server_area = client_shm_area;
            client_client_area = client_server_area + (comm_mem_per_thread / 2);
            client_server_data = (struct server_struct *) client_server_area;
            client_client_data = (struct client_struct *) client_client_area;

            if (client_client_data->dialog_counter > client_server_data->dialog_counter) {

                // read client result into general result
                fprintf(my_lfp, "client %d returned task %ld\n", thread, client_client_data->block_row_task);
                fflush(my_lfp);
                insert_client_result
                    ( client_results_base_addr
                    , thread
                    , per_client_results_shm_size
                    , result_base_addr
                    , num_elements_in_block_row_col
                    , client_client_data->block_row_task
                    );
                num_finished_block_row_tasks +=1;

                // give client next task or send done
                client_server_data->block_row_task = next_block_row_task;
                if (next_block_row_task >= num_blocks_in_matrix_row_col){
                    client_server_data->done = 1;
                }
                ret_val = msync ( client_server_area, client_server_size, MS_SYNC );
                save_errno = errno;
                if (ret_val != 0) { // error occured 
                    if (my_lfp){
                        fprintf 
                            ( my_lfp
                            , "msync initial jobs: syncing failed with errno = %d, %s\n"
                            , save_errno
                            , strerror(save_errno) 
                            );
                        fflush(my_lfp);
                    }
                }
                
                // increase dialog counter: client is ready to work on problem
                client_server_data->dialog_counter +=1;
                ret_val = msync ( client_server_area, client_server_size, MS_SYNC );
                save_errno = errno;
                if (ret_val != 0){ // error occured
                    if (my_lfp){
                        fprintf 
                            ( my_lfp
                            , "msync initial jobs dialog counter: syncing failed with errno = %d, %s\n"
                            , save_errno
                            , strerror(save_errno)
                            );
                        fflush(my_lfp);
                    }
                }
                next_block_row_task += 1;
            }
        }
    } 
}