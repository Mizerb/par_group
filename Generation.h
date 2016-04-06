#include <stdlib.h>


int* Generate_Matrix(int size, int height)
{
    int* output;
    int x;
    int arrlength = size*height
    
    output = calloc(sizeof(int), arrlength);
    
    for(x = 0; x<arrlength;x++)
    {
        output[x] = drand48()*10;
    }
    return output;
}