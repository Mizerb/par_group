#include "generation.h"
#include "info.h"


int* Generate_Matrix( int matrix_width, int matrix_height )
{
    int* output;
    int arrlength = matrix_size * matrix_slice_height;
    
    output = calloc(sizeof(int), arrlength);
    return output;
}

void Populate_Matrix(int* arr, int length)
{
    int x;
    for(x = 0; x<length;x++)
    {
        *arr = drand48()*10;
        ++arr;
    }
}