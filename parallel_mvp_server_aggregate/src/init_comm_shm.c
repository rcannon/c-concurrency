
#include "init_comm_shm.h"

void
init_comm_shm
    ( int n_threads
    , size_t mem_per_thread
    , void** shm_addr_base
    )
{
    key_t shm_key;
    int shm_id;
    size_t shm_size;
    int shm_flag;
    int ftok_number;
    int save_errno;

    system("touch /dev/shm/shm_comm");
    ftok_number = 42;
    shm_key = ftok("/dev/shm/shm_comm", ftok_number);

    shm_size = mem_per_thread * n_threads;
    shm_flag = IPC_CREAT | 0666;

    shm_id = shmget
        ( shm_key
        , shm_size
        , shm_flag
        );
    save_errno = errno;
    if (shm_id == -1){
        fprintf ( stderr
                , "shmget failed with errno: %d, %s\n"
                , save_errno
                , strerror(save_errno)
                );
        fflush(stderr);
    }
    
    attach_shm
        ( stderr
        , shm_addr_base
        , shm_id
        );
}

