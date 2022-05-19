
#include "attach_shm.h"

void
attach_shm
    ( FILE* my_lfp
    , void** shm_addr_base
    , int shm_id
    )
{
    void* shm_addr;
    int shm_flag;
    int save_errno;

    shm_addr = NULL;
    shm_flag = IPC_CREAT | 0666;

    shm_addr = NULL;
    *shm_addr_base = shmat(shm_id, shm_addr, shm_flag);
    save_errno = errno;
    if (*shm_addr_base == (void *) -1){
        if (my_lfp) {
            fprintf 
                ( my_lfp
                , "shmat failed with errno: %d, %s\n"
                , save_errno
                , strerror(save_errno)
                );
            fflush(my_lfp);
        }
    }
}