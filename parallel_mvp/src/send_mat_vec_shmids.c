
#include "send_clients_mat_vec_shmids.h"

void
send_mat_vec_shm_ids
    ( FILE* my_lfp
    , void* shm_addr_base
    , int n_threads
    , size_t mem_per_thread
    , int matrix_shm_id
    , int vector_shm_id
    )
{
    void* client_shm_area;
    void* client_server_area;
    void* client_client_area;
    volatile struct server_struct * client_server_data;
    volatile struct client_struct * client_client_data;
    size_t client_server_size;
    int client_id;

    int done;
    int msync_success;
    int ret_val;
    int save_errno;
    int my_thread_id;

    my_thread_id = 0; // only server calls this

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

            for (client_id = 1; client_id < n_threads; client_id++) {

                // access client's server and client area
                // Note: caution address arithmetic 
                client_shm_area = shm_addr_base + client_id * mem_per_thread;
                client_server_area = client_shm_area;
                client_client_area = client_server_area + (mem_per_thread / 2);
                client_server_data = (struct server_struct *) client_server_area;
                client_client_data = (struct client_struct *) client_client_area;

                // check if client is ready for task
                if (client_client_data->dialog_counter > client_server_data->dialog_counter) {

                    msync_success = 1;

                    // send matrix shm id
                    client_server_data->matrix_shm_id = matrix_shm_id;
                    ret_val = msync( client_server_area, client_server_size, MS_SYNC );
                    save_errno = errno;
                    if (ret_val != 0) { // error occured 
                        msync_success = 0;
                        if (my_lfp){
                            fprintf 
                                ( my_lfp
                                , "msync matrix shm id: syncing failed with errno = %d, %s\n"
                                , save_errno
                                , strerror(save_errno) 
                                );
                            fflush(my_lfp);
                        }
                        else { 
                            print_outfile_not_found(my_thread_id);
                        }
                    }
                    // send vector shm id
                    client_server_data->vector_shm_id = vector_shm_id;
                    ret_val = msync( client_server_area, client_server_size, MS_SYNC );
                    save_errno = errno;
                    if (ret_val != 0) { // error occured 
                        msync_success = 0;
                        if (my_lfp){
                            fprintf 
                                ( my_lfp
                                , "msync vector shm id: syncing failed with errno = %d, %s\n"
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
                        ret_val = msync( client_server_area, client_server_size, MS_SYNC );
                        save_errno = errno;
                        if (ret_val != 0){ // error occured
                            if (my_lfp){
                                fprintf 
                                    ( my_lfp
                                    , "msync server dialog counter: syncing failed with errno = %d, %s\n"
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