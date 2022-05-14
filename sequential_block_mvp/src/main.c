
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
    int result_size;
    int debug;

    if (argc != 4){
        print_string(stderr, "need three arguments: num_blocks_in_matrix_row_col, num_elements_in_block_row_col, and debug(0/1)");
    }

    else {

        num_blocks_in_matrix_row_col = atoi(argv[1]);
        num_elements_in_block_row_col = atoi(argv[2]);
        debug = atoi(argv[3]);

        build_block_matrix  ( &matrix
                            , num_blocks_in_matrix_row_col
                            , num_elements_in_block_row_col
                            , debug
                            );
        

        /*
        build_vector( &result
                    , num_blocks_in_matrix_row_col
                    , num_elements_in_block_row_col
                    , debug
                    );*/

        build_vector( &vector
                    , num_blocks_in_matrix_row_col
                    , num_elements_in_block_row_col
                    , debug
                    );

        allocate_result ( &result
                        , num_blocks_in_matrix_row_col
                        , num_elements_in_block_row_col
                        );
        
        print_string(stderr, "testing vector output\n");
        int iter;
        int vector_size = num_blocks_in_matrix_row_col * num_elements_in_block_row_col;
        for (iter = 0; iter < vector_size; iter++){
            fprintf(stderr, "%.1f ", (result)[iter]);
            fflush(stderr);
        }
        print_string(stderr, "\n");

        run_block_mvp   ( &result
                        , &matrix
                        , &vector
                        , num_blocks_in_matrix_row_col
                        , num_elements_in_block_row_col
                        );
        
        result_size = num_blocks_in_matrix_row_col * num_elements_in_block_row_col;

        if (debug) {
            debug = verify_result(&result, result_size);
            if (!debug) {
                print_string(stderr, "incorrect\n");
            }
            else {
                print_string(stderr, "correct\n");
            }
        }
        print_result(&result, result_size);
    }
    return 0;
}