#include "lib/taslock.h"
#include <stdlib.h>
#include <stdatomic.h>
#include <stdio.h>
#include <sched.h>

void* init_taslock() {
    struct Taslock* taslock = malloc(sizeof(struct Taslock));
    taslock->state = 0;
    return (void*)taslock;
}

void destroy_taslock(void* v_taslock) {
    free(v_taslock);
}

void tas_lock(void* v_taslock) {
    while (__atomic_exchange_n(&((struct Taslock*) v_taslock)->state, 1, __ATOMIC_SEQ_CST)) sched_yield();
}

int tas_trylock(void* v_taslock) {
    return !__atomic_exchange_n(&((struct Taslock*) v_taslock)->state, 1, __ATOMIC_SEQ_CST);
}

void tas_unlock(void* v_taslock) {
    __atomic_store_n(&((struct Taslock*) v_taslock)->state, 0, __ATOMIC_SEQ_CST);
}
