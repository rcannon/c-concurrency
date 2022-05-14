
#include "verify_result.h"

int
verify_result   ( double **result
                , int result_size 
                )
{
    double* result_iter;
    int correct;
    int i;
    double answer;

    correct = 1;
    result_iter = &(**result);
    print_string(stderr,"answer: ");
    for (i = 0; i < result_size; i++){
        answer = (double) result_size * (result_size-1)/2 + i * (result_size - i);
        fprintf(stderr, "%.1f ", answer);
        fflush(stderr);
        correct = correct && (*result_iter == answer);
        result_iter++;
    }

    return correct;
}