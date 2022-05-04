
#include "hypercube_fork.h"

pid_t
hypercube(int nthreads)
{
    pid_t pid;
    int my_thread_id;
    int child_thread_id;
    int step;
    int max_threads;
    int done;
    size_t mask_size;
    cpu_set_t* cpu_maskp;

    my_thread_id = 0;
    step = 1;
    done = 0;

    while (!done) {

        child_thread_id = my_thread_id + step;
        if (child_thread_id >= nthreads) {
            done = 1;
        }

        else {
            pid = fork();
            if (pid == 0){
                /* I am the child */
                my_thread_id = child_thread_id;
            }
            else if (pid == -1) {
                print_string(stderr, "forking failed");
            }
            step = step + step;
        }
    }

    /* my_core = bind(my_core,stderr); */

    /* Bind to 0th cpu core */
    max_threads = 1024;
    cpu_maskp = CPU_ALLOC(max_threads);
    mask_size = CPU_ALLOC_SIZE(max_threads);
    CPU_ZERO_S(mask_size, cpu_maskp);
    CPU_SET_S(my_thread_id, mask_size, cpu_maskp);

    pid = 0;
    sched_setaffinity(pid, mask_size, cpu_maskp);
    
    return(my_thread_id);
}