#ifndef _ALOCK_H
#define _ALOCK_H

struct Alock {
    _Atomic volatile int tail;
    int* flag;
    int size;
};

void* init_alock(int capacity);

void destroy_alock(void* v_alock);

void a_lock(void* v_alock, int* myslot);

int a_trylock(void* v_alock, int* myslot);

void a_unlock(void* v_alock, int* myslot);


#endif
