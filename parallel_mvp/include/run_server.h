
#include "system_includes.h"
#include "print_string.h"
#include "server_struct.h"
#include "client_struct.h"
#include "print_outfile_not_found.h"

#ifndef _RUN_SERVER_H_
#define _RUN_SERVER_H_ 1

void 
run_server  ( FILE* my_lfp
            , void* shm_addr_base
            , int n_threads
            , size_t mem_per_thread
            , int argc
            , char** argv
            );

#endif