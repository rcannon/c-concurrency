
#include "print_outfile_not_found.h"

void
print_outfile_not_found( int my_thread_id )
{
    fprintf(stderr, "thread %d output file no found", my_thread_id);
    fflush(stderr);
}