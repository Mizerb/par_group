#ifndef CALCULATION_H
#define CALCULATION_H

typedef struct 
{
    int mpi_rank;
    int mpi_commsize;
    int pthreads_per_mpi;
    int matrix_size;
    int matrix_slice_height;
    int* matrix_data;
    int** ghost_data;
} program_info;


void run_threadpool( void * (*func) (void *), void *arg, size_t num_threads );
void * tpool_initialize_matrix( void* args );


#endif