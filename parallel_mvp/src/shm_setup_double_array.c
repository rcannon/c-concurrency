
#include "shm_setup_double_array.h"

int
shm_setup_double_array
    ( FILE* my_lfp
    , key_t shm_key
    , size_t shm_size
    , void** shm_addr_base
    )
{
    int shm_flag;
    int save_errno;
    int my_thread_id;

    my_thread_id = 0; // only server should call this function

    shm_flag = IPC_CREAT | 0666;
    shm_size = shm_size * sizeof(double);

    // get shm segment
    shm_id = shmget(shm_key, shm_size, shm_flag);
    save_errno = errno;
    if (shm_id == -1){
        if (my_lfp) {
            fprintf 
                ( my_lfp
                , "shmget failed with errno: %d, %s\n"
                , save_errno
                , strerror(save_errno)
                );
            fflush(my_lfp);
        }
        else {
            print_outfile_not_found(my_thread_id);
        }
    }
    // attach to shm segment
    shm_addr = NULL;
    *shm_addr_base = shmat(shm_id, shm_addr, shm_flag);
    save_errno = errno;
    if (*shm_addr_base == (void *) -1){
        if (my_lfp) {
            fprintf 
                ( stderr
                , "shmat failed with errno: %d, %s\n"
                , save_errno
                , strerror(save_errno)
                );
        fflush(my_lfp);
        }  
        else {
            print_outfile_not_found(my_thread_id);
        }
    } 
    return shm_id;
}
