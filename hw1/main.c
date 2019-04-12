#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "lib/adj_mat.h"
#include "lib/floydwarshall.h"

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

    clock_t start, end;
    int t = strtol(argv[2], NULL, 10);
    switch (argv[1][0]) {
        case 's':
            start = clock();
            for (int i = 0; i < 1; i++) adj_mat = serial_fw(adj_mat);
            end = clock();
            if (!adj_mat) error_and_exit();
            break;
        case 'p':
            start = clock();
            for (int i = 0; i < 1; i++) adj_mat = parallel_fw(adj_mat, t);
            end = clock();
            if (!adj_mat) error_and_exit();
            break;
        default:
            return usage();
    }
    write_adj_mat(adj_mat, argv[4]);
    printf("%f\n", ((double)(end - start))/CLOCKS_PER_SEC);
    free_adj_mat(adj_mat);
    return 0;
}
