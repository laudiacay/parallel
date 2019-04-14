#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "lib/adj_mat.h"
#include "lib/floydwarshall.h"
#include "lib/stopwatch.h"

int usage() {
    printf("Usage:\n");
    printf("\t  Serial: ./floydwarshall s threads infile outfile\n");
    printf("\tParallel: ./floydwarshall p threads infile outfile\n");
    printf("Change # of threads in floydwarshall.c\n");
    return 1;
}

void error_and_exit() {
    printf("An error occurred!\n");
    exit(1);
}

// todo: test plan
// todo: parallel version
// todo: test hypotheses or something

int main(int argc, char *argv[]) {
    // handle bad input
    if (argc != 5) return usage();

    struct adj_mat* adj_mat = read_input_file(argv[3]);
    if (adj_mat == NULL) {
        printf("Failed to read in input file. Check formatting and existence\n");
        return 1;
    }

    int t = strtol(argv[2], NULL, 10);
    struct pthreads* pthreads;
    StopWatch_t* timer = malloc(sizeof(StopWatch_t));
    switch (argv[1][0]) {
        case 's':
            startTimer(timer);
            adj_mat = serial_fw(adj_mat);
            stopTimer(timer);
            if (!adj_mat) error_and_exit();
            break;
        case 'p':
            pthreads = prep_thread_args(t, adj_mat);
            startTimer(timer);
            adj_mat = parallel_fw(adj_mat, pthreads);
            stopTimer(timer);
            thread_teardown(pthreads);
            if (!adj_mat) error_and_exit();
            break;
        default:
            return usage();
    }
    write_adj_mat(adj_mat, argv[4]);
    printf("%f\n", getElapsedTime(timer));
    free(timer);
    free_adj_mat(adj_mat);
    return 0;
}
