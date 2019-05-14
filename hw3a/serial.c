#include <stdio.h>

void serial_experiment(long B) {
    unsigned long long increment_me = 0;
    for (long i = 0; i < B; i++) {
        increment_me++;
    }
    printf("%llu\n", increment_me);
}
