/* info.h */


#ifndef INFO_H
#define INFO_H
typedef struct
{
    int mpi_rank;
    int mpi_commsize;
    int pthreads_per_mpi;
    int matrix_size;
    int matrix_slice_height;
    double* matrix_data;
    double** ghost_data;
} program_info;

#endif