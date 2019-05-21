#ifndef _LOCK_H
#define _LOCK_H

struct lock {
    char lock_type;
    void* lock;
};

struct lock* init_lock(char lock_type, int capacity);

void destroy_lock(struct lock* lock);

void lock(struct lock* lock, int* myslot);

int trylock(struct lock* lock, int* myslot);

void unlock(struct lock* lock, int* myslot);

#endif
