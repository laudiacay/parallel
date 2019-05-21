#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <unistd.h>

#include "provided/lib/packetsource.h"
#include "provided/lib/fingerprint.h"

#include "lib/lock.h"
#include "lib/timer.h"
#include "lib/lamportQ.h"

volatile int hq_should_quit = 0;

struct hq_work_args_t {
    struct WaitFreeQueue* wfq;
    struct lock* mylock;
    int alock_slot;
};

void* homequeue_worker(void* v_hq_work_args) {
    struct hq_work_args_t* hq_work_args = (struct hq_work_args_t*) v_hq_work_args;

    struct WaitFreeQueue* wfq = hq_work_args->wfq;
    struct lock* mylock = hq_work_args->mylock;
    int alock_slot = hq_work_args->alock_slot;

    volatile Packet_t* packet;

    while (!hq_should_quit) {
        if (isempty(wfq)) {
            sleep(0);
            continue;
        }
        lock(mylock, &alock_slot);
        if (isempty(wfq)) { unlock(mylock, &alock_slot); continue; }
        while (! (packet = (volatile Packet_t*) deq(wfq))) sleep(0);
        unlock(mylock, &alock_slot);
        getFingerprint(packet->iterations, packet->seed);
        free((void*) packet);
    }

    return NULL;
}

struct hq_disp_args_t {
    struct WaitFreeQueue** wfqs;
    PacketSource_t* p_source;
    volatile Packet_t* (*getPacket)(PacketSource_t*, int);
};

void* homequeue_dispatcher(void* v_disp_args) {
    struct hq_disp_args_t* disp_args = (struct hq_disp_args_t*) v_disp_args;

    struct WaitFreeQueue** wfqs = disp_args->wfqs;
    PacketSource_t* p_source = disp_args->p_source;
    volatile Packet_t* (*getPacket)(PacketSource_t*, int) = disp_args->getPacket;

    int n = p_source->numSources;

    while (!hq_should_quit) {
        for (int t = 0; t < n; t++) {
            if (!isfull(wfqs[t]))
                assert(!enq(wfqs[t], getPacket(p_source, t)));
        }
    }
    return NULL;
}

void homequeue_exp(PacketSource_t* p_source,
    volatile Packet_t* (*getPacket)(PacketSource_t*, int),
    long m, int n, int D, char L) {

    pthread_t* threads = malloc(n * sizeof(pthread_t));
    struct WaitFreeQueue** wfqs = malloc(n*sizeof(struct WaitFreeQueue*));
    struct hq_work_args_t* hq_work_args = malloc(n*sizeof(struct hq_work_args_t));

    for (int i = 0; i < n; i++) {
        hq_work_args[i].wfq = makeWaitFreeQueue(D);
        wfqs[i] = hq_work_args[i].wfq;
        hq_work_args[i].mylock = init_lock(L, n);
        hq_work_args[i].alock_slot = i;
    }

    struct hq_disp_args_t disp_args = {wfqs, p_source, getPacket};
    pthread_t disp_thread;

    // spin up threads
    for (int i = 0; i < n; i++)
        assert(!pthread_create(&threads[i], NULL, homequeue_worker, &hq_work_args[i]));
    assert(!pthread_create(&disp_thread, NULL, homequeue_dispatcher, &disp_args));

    timer(&hq_should_quit, m);

    assert(!pthread_join(disp_thread, NULL));
    for (int t = 0; t < n; t++)
        assert(!pthread_join(threads[t], NULL));

    struct WaitFreeQueue* wfq;
    struct lock* mylock;
    for (int i = 0; i < n; i++) {
        wfq = hq_work_args[i].wfq;
        mylock = hq_work_args[i].mylock;
        while (!isempty(wfq)) free((void*) deq(wfq));
        freeWaitFreeQueue(wfq);
        destroy_lock(mylock);
    }

    free(wfqs);
    free(hq_work_args);
    free(threads);

}
