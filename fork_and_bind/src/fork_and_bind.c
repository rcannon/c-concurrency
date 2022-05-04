
#include "system_includes.h"
#include "print_string.h"
#include "hypercube_fork.h"

int 
main(int argc, char** argv) {

    if (argc != 2) {
        print_string(stdout, "expecting exactly one arguement.");
    }

    else if (atoi(argv[1]) > 1024) {
        print_string(stdout, "maximum of 1024 threads allowed.");
    }

    else {
        
        /* thread stuff */
        int nthreads;
        int my_thread_id;

        /* per thread output file stuff */
        int base_filename_length = 18;
        char* base_filename = "thread_files/out_";
        int offset;
        int file_threadname_start;
        int max_filename_length = 34; /* fits 1024 threads */
        char my_lfp_name[max_filename_length];
        FILE* my_lfp = NULL;

        /* fork the processes */
        nthreads = atoi(argv[1]);
        my_thread_id = hypercube(nthreads);
        
        /* get unique output file for each process */
        offset = snprintf(my_lfp_name, base_filename_length, base_filename);
        file_threadname_start = offset - base_filename_length;
        snprintf(my_lfp_name + offset, file_threadname_start, "%d", my_thread_id);

        while (!my_lfp){
            my_lfp = fopen(my_lfp_name, "a");
        }
        print_string(my_lfp, "hi there!");
        fflush(my_lfp);
        fclose(my_lfp);
    }

    return 0;
}