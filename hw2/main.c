#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "lib/serial.h"
#include "lib/parallel.h"
#include "lib/serialq.h"
#include "provided/lib/packetsource.h"
#include "provided/lib/stopwatch.h"

int usage() {
    printf("Usage:\n");
    printf("\t  Serial: ./packets s pd n T D W sd\n");
    printf("\tParallel: ./packets p pd n T D W sd\n");
    printf("\tSerial Q: ./packets q pd n T D W sd\n");
    printf("pd: c=constant, u=uniform, e=exponential\n");
    printf("n: number of threads (n-1 sources)\n");
    printf("T: total number packets per source\n");
    printf("D: queue depth\n");
    printf("W: work per packet\n");
    printf("sd: seed\n");
    return 1;
}

void error_and_exit() {
    printf("An error occurred!\n");
    exit(1);
}

int main(int argc, char *argv[]) {

    if (argc != 8) return usage();

    int N = (int) strtol(argv[3], NULL, 10);
    long T = strtol(argv[4], NULL, 10);
    int D = (int) strtol(argv[5], NULL, 10);
    long W = strtol(argv[6], NULL, 10);
    short seed = (short) strtol(argv[7], NULL, 10);
    char mode = argv[2][0];

    PacketSource_t* packetsource = createPacketSource(W, N-1, seed);
    StopWatch_t* timer = malloc(sizeof(StopWatch_t));

    // declare some stuff for the parallel implementation
    struct WaitFreeQueue** wfqs;
    void** w_args_arr;
    pthread_t* threads_arr;

    switch (argv[1][0]) {
        case 's':
            startTimer(timer);
            serial_experiment(packetsource, T, mode);
            stopTimer(timer);
            break;
        case 'p':
            wfqs = makeWaitFreeQueues(N-1, D);
            w_args_arr = build_worker_args(N-1, wfqs, T);
            threads_arr = malloc((N-1) * sizeof(pthread_t));

            startTimer(timer);
            parallel_experiment(packetsource, T, N-1, mode, wfqs, w_args_arr, threads_arr);
            stopTimer(timer);

            free(threads_arr);
            free_worker_args(N-1, w_args_arr);
            freeWaitFreeQueues(wfqs, N-1);
            break;
        case 'q':
            wfqs = makeWaitFreeQueues(N-1, D);

            startTimer(timer);
            serialq_experiment(packetsource, T, N-1, mode, wfqs);
            stopTimer(timer);

            freeWaitFreeQueues(wfqs, N-1);
            break;
        default:
            deletePacketSource(packetsource);
            free(timer);
            return usage();
    }

    printf("%f\n", getElapsedTime(timer));
    deletePacketSource(packetsource);
    free(timer);
    return 0;
}
