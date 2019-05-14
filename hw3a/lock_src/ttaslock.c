#include "lib/ttaslock.h"
#include <stdlib.h>
#include <stdatomic.h>
#include <stdio.h>
#include <sched.h>

void* init_ttaslock() {
    struct TTaslock* ttaslock = malloc(sizeof(struct TTaslock));
    ttaslock->state = 0;
    return (void*)ttaslock;
}

void destroy_ttaslock(void* v_ttaslock) {
    free(v_ttaslock);
}

void ttas_lock(void* v_ttaslock) {
    struct TTaslock* ttaslock = (struct TTaslock*) v_ttaslock;
    while (1) {
        while (__atomic_load_n(&ttaslock->state, __ATOMIC_SEQ_CST)) sched_yield();
        if (!__atomic_exchange_n(&ttaslock->state, 1, __ATOMIC_SEQ_CST)) return;
    }
}

int ttas_trylock(void* v_ttaslock) {
    struct TTaslock* ttaslock = (struct TTaslock*) v_ttaslock;
    return !__atomic_exchange_n(&ttaslock->state, 1, __ATOMIC_SEQ_CST);
}

void ttas_unlock(void* v_ttaslock) {
    struct TTaslock* ttaslock = (struct TTaslock*) v_ttaslock;
    __atomic_store_n(&ttaslock->state, 0, __ATOMIC_SEQ_CST);
}
