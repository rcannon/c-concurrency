
#include "system_includes.h"
#include "print_string.h"
#include "build_block_matrix.h"
#include "build_vector.h"
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
        print_string(stdout, "need three arguments: num_blocks_in_matrix_row_col, num_elements_in_block_row_col, and debug(0/1)");
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

        build_vector( &vector
                    , num_elements_in_block_row_col
                    , num_blocks_in_row_col_row_col
                    , debug
                    );

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
                print_string(stdout, "incorrect");
            }
            else {
                print_string(stdout, "correct");
            }
        }

        print_result(&result, result_size);
    }
    return 0;
}