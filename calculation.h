#ifndef CALCULATION_H
#define CALCULATION_H

#include "info.h"

/* generic "run this function on n threads at once" function */
void run_threadpool( void * (*func) (void *), void *arg, size_t num_threads );

/* initializes matrix data on slices of the matrix */
void * tpool_initialize_matrix( void* args );
/* adds matrix data to transpose data */
void * tpool_add_matrix( void* args );

/* do MPI send/receive on transpose slices */
void send_receive_chunks( program_info* pinfo );

#endif