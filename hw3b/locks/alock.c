#include "lib/alock.h"
#include <stdlib.h>
#include <stdio.h>
#include <sched.h>

#define PAD 8

void print_flags(struct Alock* alock) {
    int* flags = alock->flag;
    for (int i = 0; i < alock->size; i+=PAD) {
        printf("%d ", flags[i]);
    }
    printf("\n");
}

void* init_alock(int capacity) {
    struct Alock* alock = malloc(sizeof(struct Alock));
    alock->tail = 0;
    alock->flag = calloc(capacity, PAD*sizeof(int));
    (alock->flag)[0] = 1;
    alock->size = capacity*PAD;
    return alock;
}

void destroy_alock(void* v_alock) {
    struct Alock* alock = (struct Alock*) v_alock;
    free(alock->flag);
    free(alock);
}

void a_lock(void* v_alock, int* myslot) {
    struct Alock* alock = (struct Alock*) v_alock;
    int slot = __atomic_fetch_add(&(alock->tail), PAD, __ATOMIC_SEQ_CST) % alock->size;
    *myslot = slot;
    while (!alock->flag[slot]) sched_yield();
}

int a_trylock(void* v_alock, int* myslot) {
    struct Alock* alock = (struct Alock*) v_alock;
    if (alock->flag[alock->tail]) {
        a_lock(v_alock, myslot);
        return 1;
    }
    return 0;
}

void a_unlock(void* v_alock, int* myslot) {
    struct Alock* alock = (struct Alock*) v_alock;
    int slot = *myslot;
    alock->flag[slot] = 0;
    alock->flag[(slot + PAD) % alock->size] = 1;
}
