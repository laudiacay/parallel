#ifndef _MUTLOCK_H
#define _MUTLOCK_H

#include <pthread.h>

struct Mutlock {
    pthread_mutex_t mutex;
};

void* init_mutlock();

void destroy_mutlock(void* v_mutlock);

void mut_lock(void* v_mutlock);

int mut_trylock(void* v_mutlock);

void mut_unlock(void* v_mutlock);


#endif
