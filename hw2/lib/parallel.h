#ifndef PARALLEL
#define PARALLEL

#include "lamportQ.h"
#include "packetsource.h"

struct WorkerArgs {
    int T;
    struct WaitFreeQueue* q;
};

void** build_worker_args(int n, struct WaitFreeQueue** wfq, int T);
void free_worker_args(int n, void** wargs_array);
void parallel_experiment(PacketSource_t* packetsource, long T, int n, char mode,
        struct WaitFreeQueue** qs, void** worker_args, pthread_t* threads);

#endif
