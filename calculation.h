#ifndef CALCULATION_H
#define CALCULATION_H

#include "info.h"

void run_threadpool( void * (*func) (void *), void *arg, size_t num_threads );

void * tpool_initialize_matrix( void* args );
void * tpool_add_matrix( void* args );

void send_chunks( program_info pinfo );
void receive_chunks( program_info* pinfo );

#endif