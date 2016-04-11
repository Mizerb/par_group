#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <pthread.h>

#include "calculation.h"
#include "generation.h"

struct thread_info
{
    pthread_t id;
    int threadno;
    void * arg;
};

/* creates n-1 threads and runs a given function on itself and each thread */
void run_threadpool( void * (*func) (void *), void *arg, size_t num_threads )
{
    struct thread_info* targs = calloc( sizeof(struct thread_info), num_threads );
    struct thread_info* tptr;
    void * ret;
    
    size_t i;
    
    for ( i = 1, tptr = targs+1; i < num_threads; ++i, ++tptr )
    {
        tptr->threadno = i;
        tptr->arg = arg;
        pthread_create( &(tptr->id), NULL, func, tptr );
    }
    
    targs->threadno = 0;
    targs->arg = arg;
    targs->id = pthread_self();
    (*func)(targs);
    
    /* make sure all threads have finished before exiting */
    for ( tptr = targs+1; tptr < targs+num_threads; ++tptr )
    {
        pthread_join(tptr->threadno, &ret);
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

/* do summation */
void * tpool_add_matrix( void* args )
{
    struct thread_info* arg = args;
    program_info * pargs = arg->arg;
    int msh = pargs->matrix_slice_height;
    int i, j, strip_size = msh/pargs->pthreads_per_mpi;
    double **gptr;
    double *mptr, *aptr;
    
    for ( gptr = pargs->ghost_data, j = 0; j < pargs->mpi_commsize; ++gptr, ++j )
    {
        for ( mptr = *gptr + strip_size*arg->threadno, i = 0; i < strip_size*(arg->threadno + 1); ++gptr, ++i )
        {
            aptr = pargs->matrix_data + j*msh*msh + (i%msh)*msh + i/msh;
            /* OMG we actually add the matrix value and its transpose!!!! */
            *aptr += *mptr;
        }
    }
    
    return NULL;
}

/* send matrix ghost rows */
// MPI_Isend( *g_GOL_CELL, g_y_cell_size, MPI_DOUBLE, g_mpi_neighbors[0], tick, MPI_COMM_WORLD, &(send_request[0]) );

void send_chunks( program_info pinfo )
{
    int i, chunk_size = pinfo.matrix_size*pinfo.matrix_slice_height/pinfo.mpi_commsize;
    double* mptr;
    for ( i = 0, mptr = pinfo.matrix_data; i < pinfo.mpi_commsize; ++i, mptr += chunk_size )
    {
        MPI_Request r;
        MPI_Isend( mptr, chunk_size, MPI_DOUBLE, i, 923, MPI_COMM_WORLD, &r );
    }
    return;
}

/* get matrix ghost rows */

// MPI_Irecv( g_GOL_CELL[-1], g_y_cell_size, MPI_DOUBLE, g_mpi_neighbors[0], tick, MPI_COMM_WORLD, &(receive_request[0]) );

void receive_chunks( program_info* pinfo )
{
    int i, chunk_size = pinfo->matrix_size*pinfo->matrix_slice_height/pinfo->mpi_commsize;
    pinfo->ghost_data = calloc( sizeof(double*), pinfo->mpi_commsize );
    double** mptr;
    MPI_Request *r = calloc( sizeof(MPI_Request), pinfo->mpi_commsize ), *rptr;
    
    for ( i = 0, mptr = pinfo->ghost_data, rptr = r; i < pinfo->mpi_commsize; ++i, ++mptr, ++rptr )
    {
        *mptr = calloc( sizeof(double), chunk_size );
        MPI_Irecv( *mptr, chunk_size, MPI_DOUBLE, i, 923, MPI_COMM_WORLD, rptr );
    }
    /* wait until all requests have completed */
    for ( rptr = r; rptr < r + pinfo->mpi_commsize; ++rptr )
    {
        MPI_Wait( rptr, MPI_STATUS_IGNORE );
    }
    
    return;
}

