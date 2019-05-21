#ifndef _LAMPORTQ_H
#define _LAMPORTQ_H

#include <stdlib.h>
#include <stdio.h>

struct WaitFreeQueue {
    volatile int head;
    volatile int tail;
    volatile void** items;
    int capacity;
};

struct WaitFreeQueue* makeWaitFreeQueue(int capacity);
void freeWaitFreeQueue(struct WaitFreeQueue* wfq);

int enq(struct WaitFreeQueue* wfq, volatile void* item);
volatile void* deq(struct WaitFreeQueue* wfq);
int isfull(struct WaitFreeQueue* wfq);
int isempty(struct WaitFreeQueue* wfq);

struct WaitFreeQueue** makeWaitFreeQueues(int n_queues, int cap);
void freeWaitFreeQueues(struct WaitFreeQueue** wfqs, int n_queues);

#endif
