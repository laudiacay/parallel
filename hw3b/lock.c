#include <stdlib.h>
#include "locks/lib/mutlock.h"
#include "locks/lib/alock.h"
#include "lib/lock.h"

struct lock* init_lock(char lock_type, int capacity) {
    struct lock* lock = malloc(sizeof(struct lock));
    lock->lock_type = lock_type;
    switch (lock_type) {
        case 'm':
            lock->lock = init_mutlock();
            break;
        case 'a':
            lock->lock = init_alock(capacity);
            break;
        default:
            return NULL;
    }
    return lock;
}

void destroy_lock(struct lock* lock) {
    switch (lock->lock_type) {
        case 'm':
            destroy_mutlock(lock->lock);
            break;
        case 'a':
            destroy_alock(lock->lock);
            break;
    }
    free(lock);
}

void lock(struct lock* lock, int* myslot) {
    switch (lock->lock_type) {
        case 'm':
            mut_lock(lock->lock);
            break;
        case 'a':
            a_lock(lock->lock, myslot);
            break;
    }
}

int trylock(struct lock* lock, int* myslot) {
    switch (lock->lock_type) {
        case 'm':
            return mut_trylock(lock->lock);
        case 'a':
            return a_trylock(lock->lock, myslot);
    }
    return 0;
}

void unlock(struct lock* lock, int* myslot) {
    switch (lock->lock_type) {
        case 'm':
            mut_unlock(lock->lock);
            break;
        case 'a':
            a_unlock(lock->lock, myslot);
            break;
    }
}
