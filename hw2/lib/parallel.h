#ifndef PARALLEL
#define PARALLEL

#include "lamportQ.h"
#include "../provided/lib/packetsource.h"
#include "../provided/lib/fingerprint.h"
#include <unistd.h>
#include <pthread.h>
#include <assert.h>

struct WorkerArgs {
    int T;
    struct WaitFreeQueue* q;
};

void** build_worker_args(int n, struct WaitFreeQueue** wfq, int T);
void free_worker_args(int n, void** wargs_array);
void parallel_experiment(PacketSource_t* packetsource, long T, int n, char mode,
        struct WaitFreeQueue** qs, void** worker_args, pthread_t* threads);

#endif
