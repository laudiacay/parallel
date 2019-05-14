#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "lib/serial.h"
#include "lib/parallel.h"
#include "lib/lock.h"
#include "provided/lib/stopwatch.h"

int usage() {
    printf("Usage:\n");
    printf("\t./locks t l n b\n");
    printf("t: type of test, s=serial, p=parallel\n");
    printf("l: type of lock for parallel, t=TAS, m=Mutex, a=Anderson, c=CLHQueue\n");
    printf("n: number of worker threads\n");
    printf("b: number to which we are counting\n");
    return 1;
}

void error_and_exit() {
    printf("An error occurred!\n");
    exit(1);
}

int main(int argc, char *argv[]) {

    if (argc != 5) return usage();

    unsigned int N;
    unsigned long long B;
    unsigned long long increment_me = 0;

    if ((sscanf(argv[3], "%u", &N) != 1) || (sscanf(argv[4], "%llu", &B) != 1))
        return usage();

    struct lock* my_lock;
    pthread_t* threads;
    struct worker_args* w_args;

    StopWatch_t* timer = malloc(sizeof(StopWatch_t));

    switch (argv[1][0]) {
        case 's':
            startTimer(timer);
            serial_experiment(B);
            stopTimer(timer);
            break;
        case 'p':
            my_lock = init_lock(argv[2][0], N);
            if (!my_lock) {
                free(timer);
                return usage();
            }
            threads = malloc(N * sizeof(pthread_t));
            w_args = init_worker_args(my_lock, N, B, increment_me);

            startTimer(timer);
            parallel_experiment(N, w_args, threads);
            stopTimer(timer);

            free(threads);
            destroy_worker_args(w_args, N);
            destroy_lock(my_lock);
            break;
        default:
            free(timer);
            return usage();
    }

    printf("%f\n", getElapsedTime(timer));
    free(timer);
    return 0;
}
