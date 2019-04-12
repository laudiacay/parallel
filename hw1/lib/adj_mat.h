#ifndef ADJ_MAT
#define ADJ_MAT

struct adj_mat {
    int n;
    int** mat;
};

struct adj_mat* make_adj_mat(int n);
struct adj_mat* read_input_file(char *infile);
void free_adj_mat(struct adj_mat* adj_mat);
void print_adj_mat(struct adj_mat* adj_mat, int print_n);
void write_adj_mat(struct adj_mat* adj_mat, char* out_file);

#endif
