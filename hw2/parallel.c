#include "lib/parallel.h"

void** build_worker_args(int n, struct WaitFreeQueue** wfq, int T) {
    void** worker_args_array = malloc(n * sizeof(void*));
    struct WorkerArgs* wargs;
    for (int i = 0; i < n; i++) {
        wargs = malloc(sizeof(struct WorkerArgs));
        wargs->T = T;
        wargs->q = wfq[i];
        worker_args_array[i] = (void*) wargs;
    }
    return worker_args_array;
}

void free_worker_args(int n, void** wargs_array) {
    struct WorkerArgs* wargs;
    for (int i = 0; i < n; i++) {
        wargs = (struct WorkerArgs*) wargs_array[i];
        free(wargs);
        // does not free the queues, this is done back in main!
    }
    free(wargs_array);
}

void* parallel_worker(void* args) {
    struct WorkerArgs* w_args = (struct WorkerArgs*) args;
    int T = w_args->T;
    struct WaitFreeQueue* wfq = w_args->q;

    volatile Packet_t* packet;

    for (int i = 0; i < T; i++) {
        // spin until you get a packet
        while (! (packet = (volatile Packet_t*) deq(wfq))) sleep(0);
        // checksum it
        getFingerprint(packet->iterations, packet->seed);
        // and free it
        free((void*) packet);
    }
    // we did T packets, now we're done
    return NULL;
}

// i'm passing everything in to keep things fast.
void parallel_experiment(PacketSource_t* packetsource, long T, int n, char mode,
        struct WaitFreeQueue** qs, void** worker_args, pthread_t* threads) {

    volatile Packet_t* (*getPacket)(PacketSource_t*, int);
    long (*getCount)(PacketSource_t*, int);

    switch (mode) {
        case 'u':
            getPacket = getUniformPacket;
            getCount = getUniformCount;
            break;
        case 'e':
            getPacket = getExponentialPacket;
            getCount = getExponentialCount;
            break;
        case 'c':
            getPacket = getConstantPacket;
            getCount = getConstantCount;
            break;
        default:
            exit(0);
    }

    // spin up threads
    for (int i = 0; i < n; i++) {
        assert(!pthread_create(&threads[i], NULL, parallel_worker, worker_args[i]));
    }

    int finished_sources = 0;
    int* finished = (int*)calloc(n, sizeof(int));
    int t;

    // while we have not finished all sources
    while (finished_sources < n) {
        for (t = 0; t < n; t++) {
            // if source t is not done and t's queue is not full
            if (!finished[t] && !isfull(qs[t])) {
                // check if the packet source has generated T packets
                if (getCount(packetsource, t) < T) {
                    // if not, give the worker another
                    assert(!enq(qs[t], getPacket(packetsource, t)));
                } else {
                    // if so, increment finished_sources and mark this as done
                    finished_sources++;
                    finished[t] = 1;
                }
            }
        }
    }

    // join all the threads now that we've finished processing everything
    for (int t = 0; t < n; t++) assert(!pthread_join(threads[t], NULL));
    free(finished);
}
