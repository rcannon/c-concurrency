
#include "init_shm.h"

void
init_shm( int n_threads
        , size_t mem_per_thread
        , void** shm_addr_base
        )
{
    key_t shm_key;
    int shm_flag;
    int shm_id;
    size_t shm_size;
    int ftok_number;
    void* shm_addr;
    int save_errno;

    shm_flag = IPC_CREAT | 0666;

    system("touch /dev/shm/ftok_file");
    ftok_number = 42;
    shm_key = ftok("/dev/shm/ftok_file", ftok_number);

    shm_size = mem_per_thread * n_threads;

    shm_id = shmget(shm_key, shm_size, shm_flag);
    save_errno = errno;
    if (shm_id == -1){
        fprintf ( stderr
                , "shmget failed with errno: %d, %s\n"
                , save_errno
                , strerror(save_errno)
                );
        fflush(stderr);
    }
    
    shm_addr = NULL;
    *shm_addr_base = shmat(shm_id, shm_addr, shm_flag);
    save_errno = errno;
    if (*shm_addr_base == (void *) -1){
        fprintf ( stderr
                , "shmat failed with errno: %d, %s\n"
                , save_errno
                , strerror(save_errno)
                );
        fflush(stderr);
    }
}