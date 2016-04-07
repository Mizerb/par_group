#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <pthread.h>

#include "calculation.h"
#include "generation.h"

struct thread_info
{
    pthread_t id;
    size_t threadno;
    void * arg;
}

/* creates n-1 threads and runs a given function on itself and each thread */
void run_threadpool( void * (*func) (void *), void *arg, size_t num_threads )
{
    struct thread_info* targs = calloc( sizeof(struct thread_info), num_threads );
    struct thread_info* tptr;
    
    size_t i;
    
    for ( i = 1, tptr = targs+1; i < num_threads; ++i, ++tptr )
    {
        tptr->threadno = i;
        tptr->arg = arg;
        pthread_create( *(tptr->id), NULL, func, tptr );
    }
    
    targs->threadno = 0;
    targs->arg = arg;
    targs->id = pthread_self();
    (*func)(targs);
    
    /* make sure all threads have finished before exiting */
    for ( tptr = targs+1; tptr < targs+num_threads; ++tptr )
    {
        pthread_join(tptr->threadno);
    }

    free(targs);

    return;
}


/* calls the initialization of the matrix */
void * tpool_initialize_matrix( void* args )
{
    struct thread_info* arg = args;
    program_info * pargs = arg->arg;
    
    Populate_Matrix
    ( 
        pargs->matrix_data, 
        pargs->matrix_size*pargs->matrix_slice_height/pargs->pthreads_per_mpi
    );

    return NULL;
}

void * tpool_add_matrix( void* args )
{
    struct thread_info* arg = args;
    program_info * pargs = arg->arg;
    
}

/* send matrix ghost rows */
// MPI_Isend( *g_GOL_CELL, g_y_cell_size, MPI_DOUBLE, g_mpi_neighbors[0], tick, MPI_COMM_WORLD, &(send_request[0]) );

/* get matrix ghost rows */

// MPI_Irecv( g_GOL_CELL[-1], g_y_cell_size, MPI_DOUBLE, g_mpi_neighbors[0], tick, MPI_COMM_WORLD, &(receive_request[0]) );
  