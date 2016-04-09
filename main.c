/**
 * Code by the roadside
 * On the Roadside
 * There was a side of the road involved somewhere
 * For the roadside.
 * 
 * Aaron "Deadeye" Brooks
 * Ben "Stig" Mizera
 * Chris "The Cannon" Chappell
 * Trenton "Silver Tongue" Baldrey 
 * 
 * OR in order of last name:
 * 
 * Trenton Baldrey
 * Aaron Brooks
 * Chris Chappell
 * Ben Mizera
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <pthread.h>


#include "info.h"
#include "generation.h"
#include "calculation.h"
#include "documentation.h"

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
    MPI_Comm_size( MPI_COMM_WORLD, &(Ret.mpi_commsize) );
    MPI_Comm_rank( MPI_COMM_WORLD, &(Ret.mpi_rank) );
    
    Ret.matrix_slice_height = Ret.matrix_size / Ret.mpi_commsize;
    
    return Ret;
    
}


int main(int argc,  char* argv[])
{
    program_info inf;
    double start, end;
    /* get inputs from command line */
     
    /* INPUTS: 
        1. size of Matrix (X by X)
        2. Pthreads per MPI rank
        3. Other?
    */
    
    
    
    /* set up MPI ranks and communication */
    
    MPI_Init( &argc, &argv);
    
    /* start timer here? */
    if ( inf.mpi_rank == 0 )
    {
        /* wtime stuff */
        start = MPI_Wtime();
    }
    
    MPI_Barrier( MPI_COMM_WORLD );
    
    
    inf = grab_args(argc, argv);

    /* set up pthread pool and allocate matrix */
    inf.matrix_data = Generate_Matrix( inf.matrix_size, inf.matrix_slice_height );
    
    run_threadpool( &tpool_initialize_matrix, &inf, inf->pthreads_per_mpi );
    

    
    /* MPI send */
    
    send_chunks( inf );
    
    /* create ghost transpose rows */
    /* MPI recv */
    
    receive_chunks( &inf );
    
    /* add transpose into matrix */
    run_threadpool( &tpool_add_matrix, &inf, inf->pthreads_per_mpi );
     
    /* File output */

    Write_Out_Matrix();
    
    MPI_Barrier( MPI_COMM_WORLD );
  
    if ( inf.mpi_rank == 0 )
    {
        /* more wtime stuff */
        end = MPI_Wtime();
        printf( "%d\t%d\t%d\t%e\n", inf.matrix_size, inf.mpi_commsize, inf.pthreads_per_mpi, end-start );
    }

    MPI_Finalize();
    
    return EXIT_SUCCESS;
}