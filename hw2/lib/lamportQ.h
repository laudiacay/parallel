#ifndef LAMPORTQ
#define LAMPORTQ

struct WaitFreeQueue {
    volatile int head;
    volatile int tail;
    volatile void** items;
    int capacity;
};

struct WaitFreeQueue* makeWaitFreeQueue(int capacity);
int enq(struct WaitFreeQueue* wfq, volatile void* item);
int isfull(struct WaitFreeQueue* wfq);
volatile void* deq(struct WaitFreeQueue* wfq);
void freeWaitFreeQueue(struct WaitFreeQueue* wfq);
struct WaitFreeQueue** makeWaitFreeQueues(int n_queues, int cap);
void freeWaitFreeQueues(struct WaitFreeQueue** wfqs, int n_queues);

#endif
