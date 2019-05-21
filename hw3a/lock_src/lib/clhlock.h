#ifndef _CLHLOCK_H
#define _CLHLOCK_H

#include <stdatomic.h>

struct QNode {
    struct QNode* prev;
    int locked;
};

struct QNode* create_qnode();

void destroy_qnode(struct QNode* qnode);

struct CLHLock {
    _Atomic (struct QNode*) volatile tail;
};

void* init_clhlock();

void destroy_clhlock(void* v_clhlock);

void clh_lock(void* v_clhlock, struct QNode* myNode);

int clh_trylock(void* v_clhlock, struct QNode* myNode);

void clh_unlock(struct QNode* myNode);

#endif
