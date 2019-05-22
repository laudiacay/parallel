#include <unistd.h>
#include <pthread.h>
#include <assert.h>

#include "lib/lamportQ.h"
#include "lib/timer.h"

#include "provided/lib/packetsource.h"
#include "provided/lib/fingerprint.h"

volatile int lf_should_quit = 0;
volatile int lf_disp_done = 0;

void* lockfree_worker(void* v_wfq) {
    struct WaitFreeQueue* wfq = (struct WaitFreeQueue*) v_wfq;

    volatile Packet_t* packet;

    while (!lf_should_quit || !lf_disp_done) {
        if (isempty(wfq)) {
            sleep(0);
            continue;
        }
        if (! (packet = (volatile Packet_t*) deq(wfq))) continue;
        getFingerprint(packet->iterations, packet->seed);
        free((void*) packet);
    }

    return NULL;
}

struct lf_disp_args_t {
    struct WaitFreeQueue** wfqs;
    PacketSource_t* p_source;
    volatile Packet_t* (*getPacket)(PacketSource_t*, int);
};

void* lockfree_dispatcher(void* v_disp_args) {
    struct lf_disp_args_t* disp_args = (struct lf_disp_args_t*) v_disp_args;

    struct WaitFreeQueue** wfqs = disp_args->wfqs;
    PacketSource_t* p_source = disp_args->p_source;
    volatile Packet_t* (*getPacket)(PacketSource_t*, int) = disp_args->getPacket;

    int n = p_source->numSources;

    while (!lf_should_quit) {
        for (int t = 0; t < n; t++) {
            while (isfull(wfqs[t])) sleep(0);
            assert(!enq(wfqs[t], getPacket(p_source, t)));
        }
    }
    return NULL;
}

void lockfree_exp(PacketSource_t* p_source,
    volatile Packet_t* (*getPacket)(PacketSource_t*, int),
    long m, int n, int D) {

    pthread_t* threads = malloc(n * sizeof(pthread_t));
    struct WaitFreeQueue** wfqs = malloc(n * sizeof(struct WaitFreeQueue*));
    for (int i = 0; i < n; i++) {
        wfqs[i] = makeWaitFreeQueue(D);
    }

    struct lf_disp_args_t disp_args = {wfqs, p_source, getPacket};
    pthread_t disp_thread;

    // spin up threads
    for (int i = 0; i < n; i++)
        assert(!pthread_create(&threads[i], NULL, lockfree_worker, wfqs[i]));
    assert(!pthread_create(&disp_thread, NULL, lockfree_dispatcher, &disp_args));

    timer(&lf_should_quit, m);

    assert(!pthread_join(disp_thread, NULL));
    lf_disp_done = 1;
    for (int t = 0; t < n; t++)
        assert(!pthread_join(threads[t], NULL));

    for (int i = 0; i < n; i++) {
        while (!isempty(wfqs[i])) free((void*) deq(wfqs[i]));
        freeWaitFreeQueue(wfqs[i]);
    }
    free(wfqs);
    free(threads);
}
