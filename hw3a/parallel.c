#include "lib/lock.h"
#include "lib/parallel.h"
#include "lock_src/lib/clhlock.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void* parallel_worker(void* w_as) {
    struct worker_args* w_as_struct = (struct worker_args*) w_as;
    struct lock* my_lock = w_as_struct->my_lock;
    while (w_as_struct->increment_me < w_as_struct->B) {
        lock(my_lock, &(w_as_struct->my_slot), w_as_struct->myNode);

        if (w_as_struct->increment_me < w_as_struct->B)
            w_as_struct->increment_me++;

        unlock(my_lock, &(w_as_struct->my_slot), w_as_struct->myNode);
    }
    return NULL;
}

struct worker_args* init_worker_args(struct lock* my_lock, int N,
    unsigned long long B, unsigned long long increment_me) {
    struct worker_args* w_args = malloc(N*sizeof(struct worker_args));
    for (int i = 0; i < N; i++) {
        w_args[i].my_lock = my_lock;
        w_args[i].my_slot = i;
        w_args[i].increment_me = increment_me;
        w_args[i].B = B;
        w_args[i].myNode = create_qnode();
    }
    return w_args;
}

void destroy_worker_args(struct worker_args* w_args, int N) {
    for (int i = 0; i < N; i++) if (w_args[i].myNode) free(w_args[i].myNode);

    free(w_args);
}

void parallel_experiment(int N, struct worker_args* w_args, pthread_t* threads) {

    for (int i = 0; i < N; i++) {
        pthread_create(&threads[i], NULL, &parallel_worker, &w_args[i]);
    }
    for (int i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("%llu\n", (&w_args[0])->increment_me);
}
