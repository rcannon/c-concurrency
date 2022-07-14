
#include "run_client.h"

void
run_client  
    ( FILE* my_lfp
    , void* shm_addr_base
    , const int my_thread_id
    , const size_t comm_mem_per_thread
    , const size_t num_blocks_in_matrix_row_col
    , const size_t num_elements_in_block_row_col
    )
{
    void* my_server_area;
    void* my_client_area;
    void* matrix_addr_base;
    void* vector_addr_base;
    void* my_result_addr_base;

    size_t client_size;
    size_t my_result_shm_size;

    int matrix_shmid;
    int vector_shmid;
    int my_result_space_shmid;
    int ret_val;
    int save_errno;
    int done;
    int msync_success;

    // communication shm structs
    volatile struct server_struct * my_server_data;
    struct client_struct * my_client_data;

    my_server_area = shm_addr_base + (comm_mem_per_thread * my_thread_id); // address arithmetic
    my_client_area = my_server_area + (comm_mem_per_thread / 2); // address arithmetic

    my_server_data = (struct server_struct *) my_server_area;
    my_client_data = (struct client_struct *) my_client_area;

    client_size = sizeof(struct client_struct);

    // tell server I'm ready to receive matrix, vector, my_result_space shmids
    my_client_data->dialog_counter +=1;
    ret_val = msync 
        ( my_client_area
        , client_size
        , MS_SYNC
        );
    save_errno = errno;
    if (ret_val != 0){
        if (my_lfp){
            fprintf 
                ( my_lfp
                , "msync shmids dialog counter : syncing failed with errno = %d, %s\n"
                , save_errno
                , strerror(save_errno)
                );
            fflush(my_lfp);
        }
    }
    // wait for server to give me shmids, then save them
    while(my_server_data->dialog_counter < my_client_data->dialog_counter);
    matrix_shmid = my_server_data->matrix_shmid;
    vector_shmid = my_server_data->vector_shmid;
    my_result_space_shmid = my_server_data->client_result_shmid;

    //print_string(my_lfp, "I got task the shmids\n");

    // attach to the shared resources
    print_string(my_lfp, "begin attaching arrays\n");
    attach_shm // matrix
        ( my_lfp
        , &matrix_addr_base
        , matrix_shmid);
    attach_shm // vector
        ( my_lfp
        , &vector_addr_base
        , vector_shmid);
    attach_shm // my result space
        ( my_lfp
        , &my_result_addr_base
        , my_result_space_shmid
        );
    print_string(my_lfp, "finished attaching arrays\n");
    
    // move my_result_addr_base based on my_thread_id
    my_result_shm_size  = num_elements_in_block_row_col 
                        * sizeof(double);
    if (my_result_shm_size < 4096){
        my_result_shm_size = 4096;
    }
    else{
        my_result_shm_size = calc_mem_per_thread(my_result_shm_size);
    }
    my_result_addr_base += my_thread_id * my_result_shm_size; // CAUTION ADDRESS ARITHMETIC

    // loop client mvp
    done = 0;
    while (!done){

        // tell server I am ready for work
        my_client_data->dialog_counter += 1;
        ret_val = msync 
            ( my_client_area
            , client_size
            , MS_SYNC
            );
        save_errno = errno;
        if (ret_val != 0){
            if (my_lfp){
                fprintf 
                    ( my_lfp
                    , "msync mvp loop dialog counter : syncing failed with errno = %d, %s\n"
                    , save_errno
                    , strerror(save_errno)
                    );
                fflush(my_lfp);
            }
        }

        // wait for server to give me work
        while (my_server_data->dialog_counter < my_client_data->dialog_counter); //{ print_string(my_lfp, "Still ready\n"); }

        if (my_server_data->done){ // set done to true and exit the loop
            done = my_server_data->done;
        } 
        else { // not done, so do mvp work

            // get next block row to work on
            my_client_data->block_row_task = my_server_data->block_row_task;

            //fprintf( my_lfp, "I got task %ld\n", my_client_data->block_row_task);
            //fflush(my_lfp);

            // do mvp
            client_mvp_block_row
                ( my_client_data->block_row_task
                , matrix_addr_base
                , vector_addr_base
                , my_result_addr_base
                , num_blocks_in_matrix_row_col
                , num_elements_in_block_row_col
                );

            // give results to server
            ret_val = msync
                ( my_result_addr_base
                , my_result_shm_size
                , MS_SYNC
                );
            save_errno = errno;
            if (ret_val != 0){
                if (my_lfp){
                    fprintf 
                        ( my_lfp
                        , "msync mvp loop result pointer : syncing failed with errno = %d, %s\n"
                        , save_errno
                        , strerror(save_errno)
                        );
                    fflush(my_lfp);
                }
            }
            // give my task to server
            ret_val = msync
                ( my_client_area
                , client_size
                , MS_SYNC
                );
            save_errno = errno;
            if (ret_val != 0){
                if (my_lfp){
                    fprintf 
                        ( my_lfp
                        , "msync mvp loop block row task : syncing failed with errno = %d, %s\n"
                        , save_errno
                        , strerror(save_errno)
                        );
                    fflush(my_lfp);
                }
            }
        }
    }
    
    shmdt(matrix_addr_base);
    shmdt(vector_addr_base);
    shmdt(my_result_addr_base);
}