#ifndef FLOYDWARSHALL
#define FLOYDWARSHALL

struct pthread_params {
    struct adj_mat* adj_mat;
    int start;
    int end;
    pthread_t t;
};

struct adj_mat* serial_fw(struct adj_mat* adj_mat);
struct adj_mat* parallel_fw(struct adj_mat* adj_mat, int t_num);

#endif
