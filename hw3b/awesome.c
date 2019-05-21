#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <unistd.h>

#include "provided/lib/packetsource.h"
#include "provided/lib/fingerprint.h"

#include "lib/lock.h"
#include "lib/timer.h"
#include "lib/lamportQ.h"

volatile int aw_should_quit = 0;

struct aw_work_args_t {
    struct WaitFreeQueue** wfqs;
    struct lock** locks;
    int alock_slot;
    int n;
};

void* awesome_worker(void* v_aw_work_args) {
    struct aw_work_args_t* aw_work_args = (struct aw_work_args_t*) v_aw_work_args;

    struct WaitFreeQueue** wfqs = aw_work_args->wfqs;
    struct lock** locks = aw_work_args->locks;
    int alock_slot = aw_work_args->alock_slot;
    int n = aw_work_args->n;

    volatile Packet_t* packet;

    int cur_queue_num = alock_slot;

    struct WaitFreeQueue* wfq = wfqs[cur_queue_num];
    struct lock* mylock = locks[cur_queue_num];

    while (!aw_should_quit) {
        //printf("%d tried lock %d\n", alock_slot, cur_queue_num);
        if (!trylock(mylock, &alock_slot)) {
            cur_queue_num = (cur_queue_num + 1) % n;
            wfq = wfqs[cur_queue_num];
            mylock = locks[cur_queue_num];
            continue;
        }
        //printf("%d got lock %d\n", alock_slot, cur_queue_num);
        if (!(packet = (volatile Packet_t*) deq(wfq))) {
            unlock(mylock, &alock_slot);
            //printf("%d released lock %d\n", alock_slot, cur_queue_num);
            cur_queue_num = (cur_queue_num + 1) % n;
            wfq = wfqs[cur_queue_num];
            mylock = locks[cur_queue_num];
            continue;
        } else {
            unlock(mylock, &alock_slot);
            //printf("%d released lock %d\n", alock_slot, cur_queue_num);
            getFingerprint(packet->iterations, packet->seed);
            free((void*) packet);
        }
    }

    return NULL;
}

struct aw_disp_args_t {
    struct WaitFreeQueue** wfqs;
    PacketSource_t* p_source;
    volatile Packet_t* (*getPacket)(PacketSource_t*, int);
};

void* awesome_dispatcher(void* v_disp_args) {
    struct aw_disp_args_t* disp_args = (struct aw_disp_args_t*) v_disp_args;

    struct WaitFreeQueue** wfqs = disp_args->wfqs;
    PacketSource_t* p_source = disp_args->p_source;
    volatile Packet_t* (*getPacket)(PacketSource_t*, int) = disp_args->getPacket;

    int n = p_source->numSources;

    while (!aw_should_quit) {
        for (int t = 0; t < n; t++) {
            while (isfull(wfqs[t])) {
                sleep(0);
                //printf("stuck on queue %d\n", t);
            }
            assert(!enq(wfqs[t], getPacket(p_source, t)));
        }
    }
    return NULL;
}

void awesome_exp(PacketSource_t* p_source,
    volatile Packet_t* (*getPacket)(PacketSource_t*, int),
    long m, int n, int D, char L) {

    pthread_t* threads = malloc(n * sizeof(pthread_t));
    struct WaitFreeQueue** wfqs = malloc(n*sizeof(struct WaitFreeQueue*));
    struct lock** locks = malloc(n*sizeof(struct lock*));
    struct aw_work_args_t* aw_work_args = malloc(n*sizeof(struct aw_work_args_t));

    for (int i = 0; i < n; i++) {
        wfqs[i] = makeWaitFreeQueue(D);
        locks[i] = init_lock(L, n);
    }

    for (int i = 0; i < n; i++) {
        aw_work_args[i].wfqs = wfqs;
        aw_work_args[i].locks = locks;
        aw_work_args[i].alock_slot = i;
        aw_work_args[i].n = p_source->numSources;
    }

    struct aw_disp_args_t disp_args = {wfqs, p_source, getPacket};
    pthread_t disp_thread;

    // spin up threads
    for (int i = 0; i < n; i++)
        assert(!pthread_create(&threads[i], NULL, awesome_worker, &aw_work_args[i]));
    assert(!pthread_create(&disp_thread, NULL, awesome_dispatcher, &disp_args));

    timer(&aw_should_quit, m);

    assert(!pthread_join(disp_thread, NULL));
    for (int t = 0; t < n; t++)
        assert(!pthread_join(threads[t], NULL));

    struct WaitFreeQueue* wfq;
    struct lock* lock_i;
    for (int i = 0; i < n; i++) {
        wfq = wfqs[i];
        lock_i = locks[i];
        while (!isempty(wfq)) free((void*) deq(wfq));
        freeWaitFreeQueue(wfq);
        destroy_lock(lock_i);
    }

    free(wfqs);
    free(locks);
    free(aw_work_args);
    free(threads);

}
