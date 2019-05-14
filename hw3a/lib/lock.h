#ifndef _LOCK_H
#define _LOCK_H

#include "../lock_src/lib/clhlock.h"

struct lock {
    char lock_type;
    void* lock;
};

struct lock* init_lock(char lock_type, int capacity);

void destroy_lock(struct lock* lock);

void lock(struct lock* lock, int* myslot, struct QNode** myNode);

int trylock(struct lock* lock, int* myslot, struct QNode** myNode);

void unlock(struct lock* lock, int* myslot, struct QNode** myNode);

#endif
