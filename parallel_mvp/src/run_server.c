
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
    void* client_server_area;
    void* client_client_area;
    volatile struct server_struct * client_server_data;
    volatile struct client_struct * client_client_data;
    size_t client_server_size;
    int client_id;
    int ret_val;
    int save_errno;
    int done;

    //struct server * my_server_data;
    //struct client * my_client_data;
    client_server_size = sizeof(struct server_struct);

    done = 0;
    while (done < n_threads-1) {

        for (client_id = 1; client_id < n_threads; client_id++) {

            /* caution address arithmetic */
            client_shm_area = shm_addr_base + client_id * mem_per_thread;
            client_server_area = client_shm_area;
            client_client_area = client_server_area + (mem_per_thread / 2);

            client_server_data = (struct server_struct *) client_server_area;
            client_client_data = (struct client_struct *) client_client_area;

            // check if client is ready for task
            if (client_client_data->dialog_counter > client_server_data->dialog_counter) {

                done += 1;

                fprintf(my_lfp, "now on job %d\n", done);
                fflush(my_lfp);

                client_server_data->task = done; /* task number of next task to exec */
                
                ret_val = msync ( client_server_area
                                , client_server_size
                                , MS_SYNC
                                );
                save_errno = errno;
                if (ret_val == 0){ // no error occured with first msync
                    client_server_data->dialog_counter +=1;
                    ret_val = msync ( client_server_area
                                    , client_server_size
                                    , MS_SYNC
                                    );
                    save_errno = errno;
                    if (ret_val != 0){ // error occured with second msync
                        if (my_lfp){
                            fprintf ( my_lfp
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
                else { // error occured with first msync
                    if (my_lfp){
                        fprintf ( my_lfp
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
}