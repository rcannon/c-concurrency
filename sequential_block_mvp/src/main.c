
#include "system_includes.h"
#include "print_string.h"
#include "build_block_matrix.h"
#include "build_vector.h"
#include "allocate_result.h"
#include "run_block_mvp.h"
#include "verify_result.h"
#include "print_result.h"

int 
main(int argc, char** argv)
{
    double* matrix;
    double* vector;
    double* result;

    int num_blocks_in_matrix_row_col;
    int num_elements_in_block_row_col;
    int num_iterations;
    int iter;
    int result_size;
    int debug;

    if (argc != 5){
        print_string( stderr
                    , "need four arguments: num_blocks_in_matrix_row_col, num_elements_in_block_row_col, num_iterations, and debug(0/1)"
                    );
    }

    else {

        num_blocks_in_matrix_row_col = atoi(argv[1]);
        num_elements_in_block_row_col = atoi(argv[2]);
        num_iterations = atoi(argv[3]);
        debug = atoi(argv[4]);

        build_block_matrix  ( &matrix
                            , num_blocks_in_matrix_row_col
                            , num_elements_in_block_row_col
                            , debug
                            );

        build_vector( &vector
                    , num_blocks_in_matrix_row_col
                    , num_elements_in_block_row_col
                    , debug
                    );

        allocate_result ( &result
                        , num_blocks_in_matrix_row_col
                        , num_elements_in_block_row_col
                        );

        if ((!matrix) || (!vector) || (!result) ){
            print_string(stderr, "matrix, vector, or result not allocated");
        }
        else {

            for (iter = 0; iter < num_iterations; iter++){

                run_block_mvp   ( &result
                                , &matrix
                                , &vector
                                , num_blocks_in_matrix_row_col
                                , num_elements_in_block_row_col
                                );
            }
        
            result_size = num_blocks_in_matrix_row_col * num_elements_in_block_row_col;

            if (debug) {
                debug = verify_result(&result, result_size);
                if (!debug) {
                    print_string(stderr, "\nincorrect\n");
                }
                else {
                    print_string(stderr, "\ncorrect\n");
                }
            }
            //print_result(&result, result_size);

            print_string(stderr, "\ndone\n");
        }
    }
    return 0;
}