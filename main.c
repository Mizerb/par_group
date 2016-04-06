/**
 * Code by the roadside
 * On the Roadside
 * There was a side of the road involved somewhere
 * For the roadside.
 * 
 * Aaron Brooks
 * Ben Mizera
 * Chris Chappell
 * Trenton Baldrey 
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <pthread.h>

#include "Generation.h"
#include "Calculation.h"
#include "Documentation.h"

typedef struct
{
    int mpi_rank;
    int mpi_commsize;
    int pthreads_per_mpi;
    int matrix_size;
    int matrix_slice_height;
    int* matrix_data;
    pthread
} program_info;


/* TODO: macro for taking x,y coords and fetching matrix location */

/* TODO: macro for getting the current absolute position in matrix*/

program_info grab_args(int argc, char** argv)
{
    
    if(argc != 3)
    {
        fprintf(stderr , "Usage: \n 1: Matrix Size (2^Z  for X by X matrix\n 2: Pthreads per mpi rank (more than 0)\n");
        exit( EXIT_FAILURE);
    }
     
    
    program_info Ret;
    
    Ret.matrix_size = atoi(argv[1]);
    
    if( (Ret.matrix_size %2 != 0)  || Ret.matrix_size < 1 )
    {
        fprintf(stderr, "Matrix_Size Usage: 2^Z for the X by X matrix size\n");
        exit(EXIT_FAILURE);
    }
    
    Ret.pthreads_per_mpi = atoi(argv[2]);
    
    if( Ret.pthreads_per_mpi < 0 )
    {
        fprintf(stderr, "Pthreads per mpi rank must be at least 0\n");
        exit(EXIT_FAILURE);
    }
    
    // Other input fun;
    
    return Ret;
    
}


int main(int argc,  char* argv[])
{
    program_info inf;
  
    /* get inputs from command line */
     
    /* INPUTS: 
        1. size of Matrix (X by X)
        2. Pthreads per MPI rank
        3. Other?
    */
    
    inf = grab_args(argc, argv);
    
    /* set up MPI ranks and communication */
    
    MPI_Init( &argc, &argv);
    MPI_Comm_size( MPI_COMM_WORLD, &mpi_commsize);
    MPI_Comm_rank( MPI_COMM_WORLD, &mpi_rank);
    
    matrix_slice_height = matrix_size/mpi_commsize;
    
    MPI_Barrier( MPI_COMM_WORLD );
    /* start timer here? */
    

    /* set up pthread pool and allocate matrix */
    
    
    
    
    inf.matrix_data = Generate_Matrix(matrix_size, matrix_slice_height);
    
    
    
    /* MPI send */
    
    
    
    /* create ghost transpose rows */
     
    /* MPI recv */
    
    /* add transpose into matrix */
    Add_Matrix();
     
    /*  */
    Write_Out_Matrix();
    
    
    
    
}