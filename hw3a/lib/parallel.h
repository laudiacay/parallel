#ifndef _PARALLEL_H
#define _PARALLEL_H

#include <pthread.h>
#include "../lock_src/lib/clhlock.h"

struct worker_args {
    struct lock* my_lock;
    int my_slot;
    unsigned long long increment_me;
    unsigned long long B;
    struct QNode* myNode;
};

struct worker_args* init_worker_args(struct lock* my_lock, int N,
    unsigned long long B, unsigned long long increment_me);

void destroy_worker_args(struct worker_args* w_args, int N);

void parallel_experiment(int N, struct worker_args* w_args, pthread_t* threads);

#endif
