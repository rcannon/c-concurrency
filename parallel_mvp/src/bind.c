
#include "bind.h"

void
bind( FILE* my_lfp
    , int my_thread_id
    )
{
    pid_t pid;
    size_t mask_size;
    cpu_set_t* cpu_maskp;
    int core_bound;
    int save_errno;
    int ret_val;
    int n_running_cpus;
    int max_threads;
    int done;
    int success;

    core_bound = -1;
    /*
    From the man page on sched_setaffinity, we may not know the correct value
    for n_threads, so we determine that iteratively starting at the number of running cpus
    n_running_cpus = 1024 up to max_threads = 262144 by powers of 2 using calls to 
    sched_getaffinity, since sched_setaffinity no longer EINVAL for incorrect n_threads.
    */

    n_running_cpus = 1024;
    max_threads = 262144;

    /* 
    To avoid allocating in loops, allocate once the maximum mask size,
    and iteratively determine what n_cpus should be. This may take up to 
    2 MB in space, which is negligable in modern archetectures
    */
    
    cpu_maskp = CPU_ALLOC(max_threads);
    done = 0;
    success = 0;

    while ((n_running_cpus <= max_threads) && (!done)){
        mask_size = CPU_ALLOC_SIZE(n_running_cpus);
        CPU_ZERO_S(mask_size, cpu_maskp);
        pid = 0;
        ret_val = sched_getaffinity(pid, mask_size, cpu_maskp);
        save_errno = errno;
        if (ret_val == 0){
            done = 1;
            success = 1;
        } else {
            /* sched_setaffinity failed */
            if (save_errno == EINVAL){
                /* if it was an EINVAL failure, increment n_cpus and try again */
                n_running_cpus = n_running_cpus << 2;
            } else {
                done = 1;
                if (my_lfp){
                    fprintf( my_lfp
                           , "bind: Call to sched_getaffinity failed with errno = %d, %s\n"
                           , save_errno
                           , strerror(save_errno)
                           );
                    fflush(my_lfp);
                }
            }
        }
    }

    /*
    we found the correct n_cpus, and mask_size matches that
    and cpu_maskp is allocated for at least appropriate size.
    Now try to set our affinity.
    */

    if (success) {

        pid = 0;
        CPU_ZERO_S(mask_size, cpu_maskp);
        CPU_SET_S(my_thread_id+1, mask_size, cpu_maskp);
        ret_val = sched_setaffinity(pid, mask_size, cpu_maskp);
        save_errno = errno;
        if (ret_val == 0) {
            core_bound = where_bound(mask_size, cpu_maskp, n_running_cpus, my_lfp);
            if (my_lfp){
                fprintf( my_lfp
                       , "thread %d bound to core %d\n"
                       , my_thread_id
                       , core_bound
                       );
                fflush(my_lfp);
            }
        } else {
            if (my_lfp) {
                fprintf ( my_lfp
                        , "bind: sched_setaffinity failed with errno = %d, %s\n"
                        , save_errno
                        , strerror(save_errno)
                        );
                fflush(my_lfp);
            }
        }
    } else {
        if (!done) {
            if (my_lfp) {
                fprintf ( my_lfp
                        , "bind: failure to get matching number of running cpus with sched_getaffinity. \n"
                        );
                fflush(my_lfp);
            }
        }
    }
}