
#include "system_includes.h"

#ifndef _WHERE_BOUND_H_
#define _WHERE_BOUND_H_ 1

extern int where_bound  ( size_t mask_size 
                        , cpu_set_t * cpu_maskp
                        , int n_running_cpus
                        , FILE* my_lfp
                        );
    
#endif