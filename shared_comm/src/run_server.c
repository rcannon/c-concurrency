
#include "run_server.h"

void
run_server( FILE* my_lfp
          , void* shm_addr_base
          , int my_thread_id
          , int n_threads
          , size_t mem_per_thread
          ) 
{
    void* client_shm_area;
    int client_id;
    struct server_struct * client_server_data;
    struct client_struct * client_client_data;
    int ret_val;
    int save_errno;

    //struct server * my_server_data;
    //struct client * my_client_data;

    print_string(my_lfp, "hi again\n");

    for (client_id = 1; client_id < n_threads; client_id++) {

        /* caution address arithmetic */
        client_shm_area = shm_addr_base + client_id * mem_per_thread;

        client_server_data = (struct server_struct *) client_shm_area;
        client_client_data = (struct client_struct *) client_shm_area + mem_per_thread / 2;

        if (client_client_data->dialog_counter > client_server_data->dialog_counter) {

            client_server_data->task = 1; /* task number of next task to exec */
            ret_val = msync( &client_server_data->task
                            , 8
                            , MS_SYNC
                            );
            save_errno = errno;
            if (ret_val == 0){ // no error occures
                client_server_data->dialog_counter +=1;
                ret_val = msync( &client_server_data->dialog_counter
                               , 8
                               , MS_SYNC
                               );
                save_errno = errno;
                if (ret_val == -1){
                    if (my_lfp){
                        fprintf( my_lfp
                               , "msync: syncing failed with errno = %d, %s\n"
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
            else { // error occured with msync
                if (my_lfp){
                    fprintf( my_lfp
                           , "msync: syncing failed with errno = %d, %s\n"
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