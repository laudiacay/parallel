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

struct adj_mat* parallel_fw(struct adj_mat* adj_mat, int t_num) {
    pthread_t* threads = malloc(sizeof(pthread_t) * t_num);
    struct pthread_params** thread_args =
        malloc(sizeof(struct pthread_params) * t_num);

    assert(!pthread_barrier_init(&barrier, NULL, t_num));

    int n = adj_mat->n;
    int step = n / t_num;
    for (int t = 0; t < t_num; t++) {
        thread_args[t] = malloc(sizeof(struct pthread_params));
        thread_args[t]->adj_mat = adj_mat;
        thread_args[t]->start = step * t;
        thread_args[t]->end = step * (t + 1);
        thread_args[t]->t = t;
        assert(!pthread_create(&threads[t], NULL, do_rows, thread_args[t]));
    }

    for (int t = 0; t < t_num; t++) {
        // join pthread
        assert(!pthread_join(threads[t], NULL));
        // free its params
        free(thread_args[t]);
    }
    free(threads);
    free(thread_args);
    return adj_mat;
}
