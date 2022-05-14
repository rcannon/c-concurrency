
#include "print_result.h"

void
print_result( double** result
            , int result_size     
            )
{
    int i;
    
    print_string(stderr, "printing result.\n");
    for (i = 0; i < result_size; i++){
        fprintf(stderr, "%.1f ", (*result)[i]);
        fflush(stderr);
    }
    print_string(stderr,"\n");
}