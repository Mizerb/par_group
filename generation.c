#include "Generation.h"


int* Generate_Matrix(program_info inf)
{
    int* output;
    int x;
    int arrlength = inf.matrix_size * inf.matrix_slice_height;
    
    output = calloc(sizeof(int), arrlength);
    
    for(x = 0; x<arrlength;x++)
    {
        output[x] = drand48()*10;
    }
    return output;
}