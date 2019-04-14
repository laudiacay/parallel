#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <stdlib.h>
#include "lib/adj_mat.h"
#include "lib/floydwarshall.h"

pthread_barrier_t barrier;

struct adj_mat* serial_fw(struct adj_mat* adj_mat) {
    int n = adj_mat->n;
    int** d = adj_mat->mat;
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (d[i][j] > d[i][k] + d[k][j])
                    d[i][j] = d[i][k] + d[k][j];
            }
        }
    }
    return adj_mat;
}

void* do_rows(void* packed_params) {
    // unpack params from struct
    struct pthread_params* params = (struct pthread_params*)packed_params;
    struct adj_mat* adj_mat = params->adj_mat;
    int start = params->start;
    int end = params->end;
    // pthread_t t = params->t;

    int n = adj_mat->n;
    int** d = adj_mat->mat;
    for (int k = 0; k < n; k++) {
        for (int i = start; i < end; i++) {
            for (int j = 0; j < n; j++) {
                if (d[i][j] > d[i][k] + d[k][j])
                    d[i][j] = d[i][k] + d[k][j];
            }
        }
        pthread_barrier_wait(&barrier);
    }
    return NULL;
}

struct pthreads* prep_thread_args(int t, struct adj_mat* adj_mat) {
    struct pthreads* pthreads = malloc(sizeof(struct pthreads));
    pthread_t* ts = malloc(sizeof(pthread_t) * t);
    struct pthread_params** t_args = malloc(sizeof(struct pthread_params) * t);
    pthreads->ts = ts;
    pthreads->t_args = t_args;
    pthreads->t = t;

    int n = adj_mat->n;
    int step = n / t;

    for (int i = 0; i < t; i++) {
        t_args[i] = malloc(sizeof(struct pthread_params));
        t_args[i]->adj_mat = adj_mat;
        t_args[i]->start = step * i;
        t_args[i]->end = step * (i + 1);
        t_args[i]->t = i;
        //printf("%lu: (%d, %d)\n", t_args[i]->t, t_args[i]->start, t_args[i]->end);
    }
    
    assert(!pthread_barrier_init(&barrier, NULL, t));
    
    return pthreads;
}

void thread_teardown(struct pthreads* pts) {
    for (int i = 0; i < pts->t; i++) free(pts->t_args[i]);
    free(pts->ts);
    free(pts->t_args);
    free(pts);
}

struct adj_mat* parallel_fw(struct adj_mat* adj_mat, struct pthreads* pthreads) {
    pthread_t* ts = pthreads->ts;
    int t_num = pthreads->t;
    struct pthread_params** t_args = pthreads->t_args;

    for (int t = 0; t < t_num; t++) {
        assert(!pthread_create(&ts[t], NULL, do_rows, t_args[t]));
    }

    for (int t = 0; t < t_num; t++) assert(!pthread_join(ts[t], NULL));

    return adj_mat;
}
