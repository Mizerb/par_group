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

#include "Generation.h"
#include "Calculation.h"
#include "Documentation.h"

/* TODO: macro for taking x,y coords and fetching matrix location */

/* TODO: macro for getting the current absolute position in matrix*/


int main(int argc, const char* argc)
{
    
    int mpi_rank;
    int mpi_commsize;
    int pthreads_per_mpi;
    int matrix_size;
    int matrix_slice_height;
    int* matrix_data;
  
    /* get inputs from command line */
     
    /* INPUTS: 
        1. size of Matrix (X by X)
        2. Pthreads per MPI rank
        3. Other?
    */
    
    if(argc != 3)
    {
        fprintf(stderr , "Wrong number of inputs\n");
        return EXIT_FAILURE;
    }
     
    /* set up MPI ranks and communication */
    
    MPI_Init( &argc, &argv);
    MPI_Comm_size( MPI_COMM_WORLD, &mpi_commsize);
    MPI_Comm_rank( MPI_COMM_WORLD, &mpi_rank);
    
    matrix_slice_height = matrix_size/mpi_commsize;
    
    MPI_Barrier( MPI_COMM_WORLD );

    /* set up pthread pool and allocate matrix */
    matrix_data = Generate_Matrix(matrix_size, matrix_slice_height);
    //Where is the matrix? It's returning to nothing atm. 
    /* MPI send */
     
    /* create ghost transpose rows */
     
    /* MPI recv */
    
    /* add transpose into matrix */
    Add_Matrix();
     
    /*  */
    Write_Out_Matrix(matrix_data , mpi_rank, matrix_size, matrix_slice_height);
}