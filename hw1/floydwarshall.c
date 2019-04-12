#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lib/adj_mat.h"
#include "lib/serial.h"

int usage() {
    printf("Usage: \n\t  Serial: ./floydwarshall s infile\n\tParallel: ./floydwarshall p infile\n");
    return 1;
}

int main(int argc, char *argv[]) {
    // handle bad input
    if (argc != 3) return usage();

    struct adj_mat* adj_mat = read_input_file(argv[2]);
    if (adj_mat == NULL) {
        printf("Failed to read in input file. Check formatting and existence\n");
        return 1;
    }

    switch (argv[1][0]) {
        case 's':
            serial_fw(adj_mat);
            break;
        case 'p':
            printf("Parallel mode\n");
            break;
        default:
            return usage();
    }

    free_adj_mat(adj_mat);
    return 0;
}
