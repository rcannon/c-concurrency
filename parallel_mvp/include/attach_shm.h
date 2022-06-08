
#include "system_includes.h"

#ifndef _ATTACH_SHM_H_
#define _ATTACH_SHM_H_ 1

void
attach_shm
    ( FILE* my_lfp
    , void** shm_addr_base
    , int shm_id
    );

#endif