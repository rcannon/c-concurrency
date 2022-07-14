
#include "run_msync.h"

void
run_msync
    ( FILE* my_lfp
    , void* array_addr_base
    , size_t msync_size
    )
{
    int ret_val;
    int save_errno;

    ret_val = msync
        ( array_addr_base
        , msync_size
        , MS_SYNC
        );
    if (ret_val != 0) { // error occured 
        if (my_lfp){
            fprintf 
                ( my_lfp
                , "msync initial jobs: syncing failed with errno = %d, %s\n"
                , save_errno
                , strerror(save_errno) 
                );
            fflush(my_lfp);
        }
    }    
}
