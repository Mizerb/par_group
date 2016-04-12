#include "generation.h"
#include "info.h"


double* Generate_Matrix( int matrix_size, int matrix_slice_height )
{
    double* output;
    int arrlength = matrix_size * matrix_slice_height;
    
    output = calloc(sizeof(double), arrlength);
    return output;
}

void Populate_Matrix(double* arr, int length)
{
    int x;
    for(x = 0; x<length;x++)
    {
        *arr = drand48()*10;
        ++arr;
    }
}