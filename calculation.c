#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <pthread.h>
#include <stdio.h>

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
        //fprintf(stderr, " Created pthread with id %d.\n", tptr->threadno);
    }
    
    targs->threadno = 0;
    targs->arg = arg;
    //fprintf(stderr, " Running function on self as thread 0\n");
    targs->id = pthread_self();
    (*func)(targs);
    /* make sure all threads have finished before exiting */
    for ( tptr = targs+1; tptr < targs+num_threads; ++tptr )
    {
        //fprintf(stderr, " Attempting to join pthread with id %d.\n", tptr->threadno);
        pthread_join(tptr->id, &ret);
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
        for ( mptr = *gptr + strip_size*msh*arg->threadno, i = 0; i < strip_size*msh; ++mptr, ++i )
        {
            aptr = pargs->matrix_data + j*msh*msh + (i%msh)*msh  + arg->threadno + i/msh;
            /* OMG we actually add the matrix value and its transpose!!!! */
            *aptr += *mptr;
        }
    }
    
    return NULL;
}


/* get matrix ghost rows */

// MPI_Irecv( g_GOL_CELL[-1], g_y_cell_size, MPI_DOUBLE, g_mpi_neighbors[0], tick, MPI_COMM_WORLD, &(receive_request[0]) );

void send_receive_chunks( program_info* pinfo )
{
    int i, chunk_size = pinfo->matrix_slice_height*pinfo->matrix_slice_height;
    pinfo->ghost_data = calloc( sizeof(double*), pinfo->mpi_commsize );
    double** gptr;
    double* mptr;
    MPI_Request *r = calloc( sizeof(MPI_Request), pinfo->mpi_commsize ), *rptr;
    
    /* do Irecvs first */
    for ( i = 0, gptr = pinfo->ghost_data, rptr = r; i < pinfo->mpi_commsize; ++i, ++gptr, ++rptr )
    {
        *gptr = calloc( sizeof(double), chunk_size );
        MPI_Irecv( *gptr, chunk_size, MPI_DOUBLE, i, 923, MPI_COMM_WORLD, rptr );
    }
    
    /* do Isends after we do the receives */
    for ( i = 0, mptr = pinfo->matrix_data; i < pinfo->mpi_commsize; ++i, mptr += chunk_size )
    {
        MPI_Request r;
        MPI_Isend( mptr, chunk_size, MPI_DOUBLE, i, 923, MPI_COMM_WORLD, &r );
    }
    
    /* wait until all requests have completed */
    for ( rptr = r; rptr < r + pinfo->mpi_commsize; ++rptr )
    {
        MPI_Wait( rptr, MPI_STATUS_IGNORE );
    }
    return;
}

