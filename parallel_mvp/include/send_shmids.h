
#include "system_includes.h"
#include "print_outfile_not_found.h"
#include "print_string.h"
#include "server_struct.h"
#include "client_struct.h"

#ifndef _SEND_SHMIDS_H_
#define _SEND_SHMIDS_H_ 1

void
send_shmids
    ( FILE* my_lfp
    , void* shm_addr_base
    , int n_threads
    , size_t mem_per_thread
    , int matrix_shmid
    , int vector_shmid
    , int client_result_shmid
    );

#endif