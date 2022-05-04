
#include "system_includes.h"
#include "print_string.h"

void 
print_string(FILE* fp, char* string){
    
    if (fp){
        fprintf(fp, "%s\n", string);
        fflush(fp);
    }
}