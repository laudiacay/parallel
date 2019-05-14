#include <stdlib.h>
#include "lock_src/lib/taslock.h"
#include "lock_src/lib/ttaslock.h"
#include "lock_src/lib/mutlock.h"
#include "lock_src/lib/alock.h"
#include "lock_src/lib/clhlock.h"
#include "lib/lock.h"

struct lock* init_lock(char lock_type, int capacity) {
    struct lock* lock = malloc(sizeof(struct lock));
    lock->lock_type = lock_type;
    switch (lock_type) {
        case 't':
            lock->lock = init_taslock();
            break;
        case '2':
            lock->lock = init_ttaslock();
            break;
        case 'm':
            lock->lock = init_mutlock();
            break;
        case 'a':
            lock->lock = init_alock(capacity);
            break;
        case 'c':
            lock->lock = init_clhlock();
            break;
        default:
            return NULL;
    }
    return lock;
}

void destroy_lock(struct lock* lock) {
    switch (lock->lock_type) {
        case 't':
            destroy_taslock(lock->lock);
            break;
        case '2':
            destroy_ttaslock(lock->lock);
            break;
        case 'm':
            destroy_mutlock(lock->lock);
            break;
        case 'a':
            destroy_alock(lock->lock);
            break;
        case 'c':
            destroy_clhlock(lock->lock);
            break;
    }
    free(lock);
}

void lock(struct lock* lock, int* myslot, struct QNode** myNode) {
    switch (lock->lock_type) {
        case 't':
            tas_lock(lock->lock);
            break;
        case '2':
            ttas_lock(lock->lock);
            break;
        case 'm':
            mut_lock(lock->lock);
            break;
        case 'a':
            a_lock(lock->lock, myslot);
            break;
        case 'c':
            clh_lock(lock->lock, myNode);
            break;
    }
}

int trylock(struct lock* lock, int* myslot, struct QNode** myNode) {
    switch (lock->lock_type) {
        case 't':
            return tas_trylock(lock->lock);
        case '2':
            return ttas_trylock(lock->lock);
        case 'm':
            return mut_trylock(lock->lock);
        case 'a':
            return a_trylock(lock->lock, myslot);
        case 'c':
            return clh_trylock(lock->lock, myNode);
    }
    return 0;
}

void unlock(struct lock* lock, int* myslot, struct QNode** myNode) {
    switch (lock->lock_type) {
        case 't':
            tas_unlock(lock->lock);
            break;
        case '2':
            ttas_unlock(lock->lock);
            break;
        case 'm':
            mut_unlock(lock->lock);
            break;
        case 'a':
            a_unlock(lock->lock, myslot);
            break;
        case 'c':
            clh_unlock(myNode);
            break;
    }
}
