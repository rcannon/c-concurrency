
#include "system_includes.h"
#include "print_string.h"
#include "calc_mem_per_thread.h"
#include "hypercube_fork.h"
#include "server_struct.h"
#include "run_server.h"
#include "run_client.h"
#include "client_struct.h"
#include "init_shm.h"

int 
main(int argc, char** argv) {

    /* thread */
    int nthreads;
    int my_thread_id;

    /* per thread output file */
    int base_filename_length = 18;
    char* base_filename = "thread_files/out_";
    int offset;
    int file_threadname_start;
    char my_lfp_name[4096]; /* big enough */

    /* shared memory - communication */
    void* shm_addr_base;
    size_t mem_per_thread;
    size_t min_shm_size_per_client;

    // matrix attributes
    size_t num_blocks_in_matrix_row_col;
    size_t num_elements_in_block_row_col;

    if (argc != 4) {
        print_string(stderr, "expecting exactly three arguements : num threads, num blocks in row/col, num elelents in block row col.");
    }

    else if (atoi(argv[1]) > 64) { // 1024
        print_string(stderr, "maximum of 64 threads allowed.");
    }

    else {

        // get number of threads to run on
        nthreads = atoi(argv[1]);
        num_blocks_in_matrix_row_col = atoi(argv[2]);
        num_elements_in_block_row_col = atoi(argv[3]);
        
        // set up shared memory
        min_shm_size_per_client = sizeof(struct server_struct) 
                                + sizeof(struct client_struct);
        mem_per_thread = calc_mem_per_thread(min_shm_size_per_client);
        fprintf(stdout, "mem_per_thread: %ld bytes", mem_per_thread);
        fflush(stdout);

        shm_addr_base = (void *) -1;
        init_shm(nthreads, mem_per_thread, &shm_addr_base);

        // fork process
        my_thread_id = hypercube(nthreads);
        
        /* get and open unique output file for each process */
        base_filename = "thread_files/out_";
        FILE* my_lfp = NULL;
        offset = snprintf(my_lfp_name, base_filename_length, base_filename);
        file_threadname_start = offset - base_filename_length;
        snprintf(my_lfp_name + offset, file_threadname_start, "%d", my_thread_id);
        while (!my_lfp){
            my_lfp = fopen(my_lfp_name, "a");
        }

        /* main */
        print_string(my_lfp, "hi there!");
        fflush(my_lfp);

        if (my_thread_id == 0){
            run_server
                ( my_lfp
                , shm_addr_base
                , nthreads
                , mem_per_thread
                , num_blocks_in_matrix_row_col
                , num_elements_in_block_row_col
                );
        }
        else {
            run_client
                ( my_lfp
                , shm_addr_base
                , my_thread_id
                , mem_per_thread
                , num_blocks_in_matrix_row_col
                , num_elements_in_block_row_col
                );
        }

        /* clean up */
        fclose(my_lfp);
        shmdt(shm_addr_base);
    }
    
    return 0;
}