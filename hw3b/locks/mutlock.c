#include "lib/mutlock.h"
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

void* init_mutlock() {
    struct Mutlock* mut_struct = malloc(sizeof(struct Mutlock));
    pthread_mutex_init(&mut_struct->mutex, NULL);
    return (void*) mut_struct;
}

void destroy_mutlock(void* v_mutlock) {
    struct Mutlock* mutlock = (struct Mutlock*) v_mutlock;
    pthread_mutex_destroy(&mutlock->mutex);
    free(mutlock);
}

void mut_lock(void* v_mutlock) {
    struct Mutlock* mutlock = (struct Mutlock*) v_mutlock;
    pthread_mutex_lock(&mutlock->mutex);
}

int mut_trylock(void* v_mutlock) {
    struct Mutlock* mutlock = (struct Mutlock*) v_mutlock;
    return !pthread_mutex_trylock(&mutlock->mutex);
}

void mut_unlock(void* v_mutlock) {
    struct Mutlock* mutlock = (struct Mutlock*) v_mutlock;
    pthread_mutex_unlock(&mutlock->mutex);
}
