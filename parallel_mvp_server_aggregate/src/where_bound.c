
#include "where_bound.h"

int 
where_bound ( size_t mask_size
            , cpu_set_t* cpu_maskp
            , int n_running_cpus
            , FILE* my_lfp
            )
{
    /*
    Determine which core a thread is bound to.
    mask_size is the output from a CPU_ALLOC_SIZE(n_running_cpus) call,
    cpu_maskp is a pointer to cpu_mask with  CPU_ALLOC(n_running_cpus), and 
    set with a CPU_SET_S call. n_running_cpussi the number of cpus
    available to run on as determinedin calling the routiem "bind".
    */
    
    int i;
    int core_bound;
    int pid;
    int ret_val;
    int save_errno;

    core_bound = -1;
    pid = 0;
    CPU_ZERO_S(mask_size, cpu_maskp);
    ret_val = sched_getaffinity(pid, mask_size, cpu_maskp);
    save_errno = errno;

    if (ret_val == 0) {
        for (i = 0; (i < n_running_cpus) && (core_bound < 0); i++ ) {
            if (CPU_ISSET_S(i, mask_size, cpu_maskp)) {
                core_bound = i;
            }
        }
    } else {
        if (my_lfp) {
            fprintf ( my_lfp
                    , "where_bound: Error sched_getaffinity call failed, error was %d, %s\n"
                    , save_errno
                    , strerror(save_errno)
                    );
            fflush(my_lfp);
        }
    }
    return (core_bound);
}