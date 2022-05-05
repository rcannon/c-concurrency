
#include "run_client.h"

void
run_client( FILE* my_lfp
          , void* shm_addr_base
          , int my_thread_id
          , size_t mem_per_thread
          )
{
    //fprintf(my_lfp, "mem per thread = %ld\n", mem_per_thread);
    //fflush(my_lfp);
    int done;
    void* my_server_area;
    void* my_client_area;
    int ret_val;
    int save_errno;
    //size_t client_dialog_counter_size;
    size_t client_size;

    volatile struct server_struct * my_server_data;
    volatile struct client_struct * my_client_data;

    my_server_area = shm_addr_base + (mem_per_thread * my_thread_id); // address arithmetic
    my_server_data = (struct server_struct *) my_server_area;

    my_client_area = my_server_area + (mem_per_thread / 2); // address arithmetic
    //fprintf(my_lfp, "diff = %ld\n", my_client_area - shm_addr_base);
    //fflush(my_lfp);
    my_client_data = (struct client_struct *) my_client_area;
    //client_dialog_counter_size = sizeof(my_client_data->dialog_counter);
    client_size = sizeof(struct client_struct);

    done = 0;

    while (!done) {

        // tell server ready for task
        my_client_data->dialog_counter +=1;

        ret_val = msync ( my_client_area
                        , client_size
                        , MS_SYNC
                        );
        save_errno = errno;
        if (ret_val != 0){
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

        // wait for server to give me task
        while(my_server_data->dialog_counter < my_client_data->dialog_counter);

        // do task
        my_client_data->task = my_server_data->task;
        fprintf(my_lfp, "I got task %ld\n", my_client_data->task);
        fflush(my_lfp);
        done = 1;
    }
}