
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

    /* shared memory */
    void* shm_addr_base;
    size_t mem_per_thread;
    size_t min_shm_size_per_client;

    if (argc != 2) {
        print_string(stdout, "expecting exactly one arguement.");
    }

    else if (atoi(argv[1]) > 1024) {
        print_string(stdout, "maximum of 1024 threads allowed.");
    }

    else {

        // get number of threads to run on
        nthreads = atoi(argv[1]);
        
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
            run_server(my_lfp, shm_addr_base, my_thread_id, nthreads, mem_per_thread);
        }
        else {
            run_client(my_lfp, shm_addr_base, my_thread_id, mem_per_thread);
        }

        /* clean up */
        fclose(my_lfp);
        shmdt(shm_addr_base);
    }
    
    return 0;
}