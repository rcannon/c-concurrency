
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
    
    for (i = 0; i < result_size; i++){
        answer = (double) result_size*(result_size+1)/2 - i*(i+1)/2 - i*(i-1)/2 + n*i;
        correct = correct && (*result_iter == answer);
        result_iter++;
    }

    return correct;
}