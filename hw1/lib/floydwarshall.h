#ifndef FLOYDWARSHALL
#define FLOYDWARSHALL

struct pthread_params {
    struct adj_mat* adj_mat;
    int start;
    int end;
    pthread_t t;
};

struct pthreads {
    pthread_t* ts;
    struct pthread_params** t_args;
    int t;
};

struct adj_mat* serial_fw(struct adj_mat* adj_mat);
struct adj_mat* parallel_fw(struct adj_mat* adj_mat, struct pthreads* pthreads);
void thread_teardown(struct pthreads* pts);
struct pthreads* prep_thread_args(int t, struct adj_mat* adj_mat);
#endif
