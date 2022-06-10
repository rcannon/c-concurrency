
#include "send_shmids.h"

void
send_shmids
    ( FILE* my_lfp
    , void* shm_addr_base
    , int n_threads
    , size_t mem_per_thread
    , int matrix_shmid
    , int vector_shmid
    , int client_result_shmid
    )
{
    void* client_shm_area;
    void* client_server_area;
    void* client_client_area;
    struct server_struct * client_server_data;
    volatile struct client_struct * client_client_data;
    size_t client_server_size;
    size_t half_mem_per_thread;
    int client_id;

    int done;
    int msync_success;
    int ret_val;
    int save_errno;
    int my_thread_id;

    my_thread_id = 0; // only server calls this
    client_server_size = sizeof(struct server_struct);
    half_mem_per_thread = mem_per_thread / 2;

    if ((shm_addr_base == (void *) -1) || (!shm_addr_base)){
        if (my_lfp){
            print_string
                ( my_lfp
                , "communication shm_addr_base is -1"
                );
            fflush(my_lfp);
        }
        else { 
            print_outfile_not_found(my_thread_id);
        }
    }
    else { // shm_addr_base is valid

        done = 0;
        while (done < n_threads-1) {

            // caution address arithmetic
            client_server_area = shm_addr_base;
            client_client_area = client_server_area + half_mem_per_thread;

            for (client_id = 1; client_id < n_threads; client_id++) {

                // access client's server and client area
                // Note: caution address arithmetic 
                client_server_area = client_client_area + half_mem_per_thread;
                client_client_area = client_server_area + half_mem_per_thread;
                client_server_data = (struct server_struct *) client_server_area;
                client_client_data = (struct client_struct *) client_client_area;

                // check if client is ready for task
                if (client_client_data->dialog_counter > client_server_data->dialog_counter) {

                    msync_success = 1;

                    // send shmids
                    client_server_data->matrix_shmid = matrix_shmid;
                    client_server_data->vector_shmid = vector_shmid;
                    client_server_data->client_result_shmid = client_result_shmid;
                    ret_val = msync ( client_server_area
                                    , client_server_size
                                    , MS_SYNC 
                                    );
                    save_errno = errno;
                    if (ret_val != 0) { // error occured 
                        msync_success = 0;
                        if (my_lfp){
                            fprintf 
                                ( my_lfp
                                , "msync share shmids: syncing failed with errno = %d, %s\n"
                                , save_errno
                                , strerror(save_errno) 
                                );
                            fflush(my_lfp);
                        }
                        else { 
                            print_outfile_not_found(my_thread_id);
                        }
                    }
                    if (msync_success){ 
                        // increase dialog counter: client is ready to work on problem
                        done += 1;
                        client_server_data->dialog_counter +=1;
                        ret_val = msync ( client_server_area
                                        , client_server_size
                                        , MS_SYNC 
                                        );
                        save_errno = errno;
                        if (ret_val != 0){ // error occured
                            if (my_lfp){
                                fprintf 
                                    ( my_lfp
                                    , "msync share shmids server dialog counter: syncing failed with errno = %d, %s\n"
                                    , save_errno
                                    , strerror(save_errno)
                                    );
                                fflush(my_lfp);
                            }
                            else {
                                print_outfile_not_found(my_thread_id);
                            }
                        }
                    }
                }
            }
        }
    }
}