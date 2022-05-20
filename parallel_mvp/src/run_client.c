
#include "run_client.h"

void
run_client( FILE* my_lfp
          , void* shm_addr_base
          , int my_thread_id
          , size_t mem_per_thread
          )
{
    void* my_server_area;
    void* my_client_area;
    void* matrix_addr_base;
    void* vectro_addr_base;

    size_t client_size;

    int matrix_shmid;
    int vector_shmid;
    int ret_val;
    int save_errno;

    volatile struct server_struct * my_server_data;
    volatile struct client_struct * my_client_data;

    my_server_area = shm_addr_base + (mem_per_thread * my_thread_id); // address arithmetic
    my_server_data = (struct server_struct *) my_server_area;

    my_client_area = my_server_area + (mem_per_thread / 2); // address arithmetic
    my_client_data = (struct client_struct *) my_client_area;

    client_size = sizeof(struct client_struct);

    //
    // get the matrix and vector shmids
    //

    // tell server I'm ready to receive shmids
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
    print_string(my_lfp, "I got task the shmids\n");

    // attach to the shared resources
    attach_shm // matrix
        ( my_lfp
        , &matrix_addr_base
        , matrix_shmid);
    attach_shm // vector
        ( my_lfp
        , &vector_addr_base
        , vector_shmid);
    
    shmdt(matrix_addr_base);
    shmdt(vector_addr_base);
}