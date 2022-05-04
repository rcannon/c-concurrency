
#include "run_client.h"

void
run_client( FILE* my_lfp
          , void* shm_addr_base
          , int my_thread_id
          , size_t mem_per_thread
          )
{
    int done;
    void* my_server_area;
    void* my_client_area;
    int ret_val;
    int save_errno;
    size_t client_struct_size;

    volatile struct server_struct * my_server_data;
    volatile struct client_struct * my_client_data;

    // caution address arithmetic
    my_server_area = shm_addr_base + (mem_per_thread * my_thread_id);
    my_server_data = (struct server_struct *) my_server_area;
    print_string(my_lfp, "hi again\n");

    my_client_area = my_server_area + (mem_per_thread / 2);
    my_client_data = (struct client_struct *) my_client_area;
    print_string(my_lfp, "hi again\n");
    my_client_data->dialog_counter = 0;
    client_struct_size = sizeof(struct client_struct);

    done = 0;

    while (!done) {

        my_client_data->dialog_counter +=1;
        while(my_server_data->dialog_counter < my_client_data->dialog_counter);

        my_client_data->task = my_server_data->task;
        fprintf(my_lfp, "I got task %ld\n", my_client_data->task);
        fflush(my_lfp);
        done = 1;

        /* tell server that task is finished */
        my_client_data->dialog_counter += 1;
        
        ret_val = msync ( my_client_area
                        , client_struct_size
                        , MS_SYNC
                        );
        save_errno = errno;
        if (ret_val == -1){
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