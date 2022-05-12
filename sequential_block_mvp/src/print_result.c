
void
print_result( double** result
            , int result_size     
            )
{
    int i;
    
    print_string("printing result.\n");
    for (i = 0; i < result_size; i++){
        fprintf(stdout, "%.1d ", (*result)[i]);
        flush(stdout);
    }
    print_string("\n");
}