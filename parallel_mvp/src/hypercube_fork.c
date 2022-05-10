
#include "hypercube_fork.h"

pid_t
hypercube(int nthreads)
{
    pid_t pid;
    int my_thread_id;
    int child_thread_id;
    int step;
    int done;

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
                /* fork failed */
                print_string(stderr, "forking failed");
            }
            step = step + step;
        }
    }

    bind(stderr, my_thread_id); 

    return(my_thread_id);
}