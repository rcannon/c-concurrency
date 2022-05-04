
#include "system_includes.h"
#include "print_string.h"
#include "server_struct.h"
#include "client_struct.h"
#include "print_outfile_not_found.h"

#ifndef _RUN_CLIENT_H_
#define _RUN_CLIENT_H_ 1

void 
run_client( FILE* my_lfp
          , void* shm_addr_base
          , int my_thread_id
          , size_t mem_per_thread
          );

#endif