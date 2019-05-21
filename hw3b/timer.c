#include <time.h>

// got some help from https://stackoverflow.com/questions/1157209/is-there-an-alternative-sleep-function-in-c-to-milliseconds
void timer(volatile int* stop_flag, unsigned long milli_sleep_len) {
    struct timespec ts;
    ts.tv_sec = milli_sleep_len / 1000;
    ts.tv_nsec = (milli_sleep_len % 1000) * 1000000;
    nanosleep(&ts, NULL);
    *stop_flag = 1;
}
