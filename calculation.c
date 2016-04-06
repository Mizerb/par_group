#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <pthread.h>


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
        
    }

    return;
}