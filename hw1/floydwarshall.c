#include <stdlib.h>
#include <stdio.h>

int usage() {
    printf("Usage: \n\tSerial: ./floydwarshall s infile\n\tParallel ./floydwarshall p infile\n");
    return 1;
}

int** read_input_file(char *infile) {
    FILE* f = fopen(infile, "r");
    if (f == NULL) {
        return NULL;
    }
    
    char* n_buf = malloc(50);
    
    size_t i = 0;
    int c;
    while ((c = fgetc(f)) != EOF) {
        n_buf[i++] = (char) c;
        printf("%x\n", n_buf[i]);
        if (n_buf[i] == '\n') break;
    }
    n_buf[i] = '\0';
    
    char* extra;
    int n = strtol(n_buf, &extra, 10);
    free(n_buf);

    printf("N: %d\n", n);

    fclose(f);
    return NULL;
}

int main(int argc, char *argv[]) {
    // handle bad input
    if (argc != 3)
        return usage();
    
    int **adj_mat = read_input_file(argv[2]);
    if (adj_mat == NULL) {
        printf("Failed to read in input file. Check formatting and existence\n");
        return 1;
    }

    switch (argv[1][0]) {
        case 's':
            printf("Serial mode\n");
            break;
        
        case 'p':
            printf("Parallel mode\n");
            break;

        default:
            return usage();
    }

    return 0;
}
