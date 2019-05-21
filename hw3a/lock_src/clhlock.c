#include "lib/clhlock.h"
#include <stdlib.h>
#include <stdatomic.h>

struct QNode* create_qnode() {
    struct QNode* qnode = malloc(sizeof(struct QNode));
    qnode->prev = NULL;
    qnode->locked = 0;
    return qnode;
}

void destroy_qnode(struct QNode* qnode) {
    if (qnode) free(qnode);
}

void* init_clhlock() {
    struct CLHLock* clhlock = malloc(sizeof(struct CLHLock*));
    clhlock->tail = create_qnode();
    return (void*) clhlock;
}

void destroy_clhlock(void* v_clhlock) {
    struct CLHLock* clhlock = (struct CLHLock*) v_clhlock;
    destroy_qnode(clhlock->tail);
    free(clhlock);
}

void clh_lock(void* v_clhlock, struct QNode** myNode) {
    struct CLHLock* clhlock = (struct CLHLock*) v_clhlock;
    printf("enqueueing thread %d\n", );
    (*myNode)->locked = 1;
    (*myNode)->prev = atomic_exchange(&(clhlock->tail), *myNode);
    while ((*myNode)->prev->locked);
    //free((*myNode)->prev);
}


// TODO: wrong, redo me
int clh_trylock(void* v_clhlock, struct QNode** myNode) {
    if (!((*myNode)->prev)->locked) {
        clh_lock(v_clhlock, myNode);
        return 1;
    }
    return 0;
}

void clh_unlock(struct QNode** myNode) {
    (*myNode)->locked = 0;
    *myNode = (*myNode)->prev;
}
