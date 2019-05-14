#ifndef _TTASLOCK_H
#define _TTASLOCK_H

#include <stdatomic.h>

struct TTaslock {
    _Atomic volatile int state;
};

void* init_ttaslock();

void destroy_ttaslock(void* v_ttaslock);

void ttas_lock(void* v_ttaslock);

int ttas_trylock(void* v_ttaslock);

void ttas_unlock(void* v_ttaslock);


#endif
