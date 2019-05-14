#ifndef _TASLOCK_H
#define _TASLOCK_H

#include <stdatomic.h>

struct Taslock {
    _Atomic volatile int state;
};

void* init_taslock();

void destroy_taslock(void* v_taslock);

void tas_lock(void* v_taslock);

int tas_trylock(void* v_taslock);

void tas_unlock(void* v_taslock);


#endif
