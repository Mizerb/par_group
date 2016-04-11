#include "generation.h"
#include "info.h"


double* Generate_Matrix( int matrix_width, int matrix_height )
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