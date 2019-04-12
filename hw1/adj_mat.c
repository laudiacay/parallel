#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "lib/adj_mat.h"

/*
ASSUMPTIONS ABOUT PARSING:
no negative integers
lines are separated by \n
as much whitespace as you want can be in the file, but at least one ' ' btwn #s
0's on the diagonal
10000000 means infinite
*/


char* read_until_newline(FILE* f, int n) {
    int input_buffer_length = 100;
    if (n) input_buffer_length = 9*n + 10;
    char* n_buf = malloc(input_buffer_length);

    size_t i = 0;
    int c;
    while ((c = fgetc(f)) != '\n') {
        n_buf[i++] = (char) c;
        if (n_buf[i] == '\n') break;
    }
    n_buf[i] = '\0';

    return n_buf;
}

struct adj_mat* make_adj_mat(int n) {
    int** mat = malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) mat[i] = malloc(n*sizeof(int));

    struct adj_mat* ret = malloc(sizeof(struct adj_mat));
    ret->n = n;
    ret->mat = mat;

    return ret;
}

struct adj_mat* read_input_file(char *infile) {
    FILE* f = fopen(infile, "r");
    if (f == NULL) return NULL;

    char* line = read_until_newline(f, 0);

    int n = strtol(line, NULL, 10);
    free(line);

    struct adj_mat* ret = make_adj_mat(n);
    char* saveptr;
    char* tok;
    char delim[3] = "\t ";
    int j;
    for (int i = 0; i < n; i++) {
        line = read_until_newline(f, n);

        j = 0;
        tok = strtok_r(line, delim, &saveptr);
        while (tok != NULL) {
            ret->mat[i][j] = strtol(tok, NULL, 10);
            tok = strtok_r(NULL, delim, &saveptr);
            j++;
        }
        free(line);
    }

    fclose(f);
    return ret;
}

void free_adj_mat(struct adj_mat* adj_mat) {
    for (int i = 0; i < adj_mat->n; i++) {
        free(adj_mat->mat[i]);
    }
    free(adj_mat->mat);
    free(adj_mat);
}

void print_adj_mat(struct adj_mat* adj_mat, int print_n) {
    if (print_n) printf("n: %d\n", adj_mat->n);
    for (int i = 0; i < adj_mat->n; i++) {
        for (int j = 0; j < adj_mat->n; j++) {
            printf("%d ", adj_mat->mat[i][j]);
        }
        printf("\n");
    }
}

void write_adj_mat(struct adj_mat* adj_mat, char* out_file) {
    FILE *fp = fopen(out_file, "w");
    assert(fp);
    for (int i = 0; i < adj_mat->n; i++) {
        for (int j = 0; j < adj_mat->n; j++) {
            fprintf(fp, "%d ", adj_mat->mat[i][j]);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
}
