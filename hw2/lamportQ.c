#include "lib/lamportQ.h"
#include <stdlib.h>
#include <stdio.h>

struct WaitFreeQueue* makeWaitFreeQueue(int capacity) {
    struct WaitFreeQueue* wfq = malloc(sizeof(struct WaitFreeQueue));
    wfq->head = 0;
    wfq->tail = 0;
    wfq->items = (volatile void**) malloc(sizeof(void*) * capacity);
    wfq->capacity = capacity;
    return wfq;
}

// return 0 for success, 1 for fail
int enq(struct WaitFreeQueue* wfq, volatile void* item) {
    if (wfq->tail - wfq->head == wfq->capacity) return 1;
    __sync_synchronize();
    wfq->items[wfq->tail % wfq->capacity] = item;
    __sync_synchronize();
    wfq->tail++;
    return 0;
}

// returns 1 if full, else 0
int isfull(struct WaitFreeQueue* wfq) {
    return (wfq->tail - wfq->head == wfq->capacity);
}

// return item for success, NULL for fail
volatile void* deq(struct WaitFreeQueue* wfq) {
    if (wfq->tail - wfq->head == 0) return NULL;
    __sync_synchronize();
    void* item = (void*) wfq->items[wfq->head % wfq->capacity];
    __sync_synchronize();
    wfq->head++;
    return item;
}

// DOES NOT FREE THE THINGS IN ITEMS!!
void freeWaitFreeQueue(struct WaitFreeQueue* wfq) {
    free(wfq->items);
    free(wfq);
}

struct WaitFreeQueue** makeWaitFreeQueues(int n_queues, int cap) {
    struct WaitFreeQueue** wfqs = malloc(n_queues * sizeof(struct WaitFreeQueue*));
    for (int i = 0; i < n_queues; i++) {
        wfqs[i] = makeWaitFreeQueue(cap);
    }
    return wfqs;
}

void freeWaitFreeQueues(struct WaitFreeQueue** wfqs, int n_queues) {
    for (int i = 0; i < n_queues; i++){
        freeWaitFreeQueue(wfqs[i]);
    }
    free(wfqs);
}
