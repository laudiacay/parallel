#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/adj_mat.h"

#define INPUT_BUFFER_LENGTH 4096

char* read_until_newline(FILE* f) {
    char* n_buf = malloc(INPUT_BUFFER_LENGTH);

    size_t i = 0;
    int c;
    while ((c = fgetc(f)) != '\n') {
        n_buf[i++] = (char) c;
        if (n_buf[i] == '\n') break;
    }
    n_buf[i] = '\0';

    return n_buf;
}

struct adj_mat* read_input_file(char *infile) {
    FILE* f = fopen(infile, "r");
    if (f == NULL) return NULL;

    char* line = read_until_newline(f);

    int n = strtol(line, NULL, 10);
    free(line);

    int** mat = malloc(n * sizeof(int*));
    char* saveptr;
    char* tok;
    char delim[3] = "\t ";
    int j;
    for (int i = 0; i < n; i++) {
        mat[i] = malloc(n*sizeof(int));
        line = read_until_newline(f);

        j = 0;
        tok = strtok_r(line, delim, &saveptr);
        while (tok != NULL) {
            mat[i][j] = strtol(tok, NULL, 10);
            tok = strtok_r(NULL, delim, &saveptr);
            j++;
        }
        free(line);
    }

    fclose(f);

    struct adj_mat* ret = malloc(sizeof(struct adj_mat));
    ret->mat = mat;
    ret->n = n;

    return ret;
}

void free_adj_mat(struct adj_mat* adj_mat) {
    for (int i = 0; i < adj_mat->n; i++) {
        free(adj_mat->mat[i]);
    }
    free(adj_mat->mat);
    free(adj_mat);
}

void print_adj_mat(struct adj_mat* adj_mat) {
    printf("n: %d\n", adj_mat->n);
    for (int i = 0; i < adj_mat->n; i++) {
        for (int j = 0; j < adj_mat->n; j++) {
            printf("%d ", adj_mat->mat[i][j]);
        }
        printf("\n");
    }
}
