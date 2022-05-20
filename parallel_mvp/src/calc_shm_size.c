
#include "calc_mem_per_thread.h"

size_t
calc_shm_size( size_t min_mem_size )
{
    size_t shm_size;

    // find the right amount of memory per thread
    // lowest power of 2 for alignment
    shm_size = 2 * 4096; // two times usual linux PAGESIZE
    while (shm_size < min_mem_size){
        shm_size = shm_size << 1;
    }

    return shm_size;
}