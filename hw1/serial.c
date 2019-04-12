#include <stdio.h>
#include "lib/adj_mat.h"

void serial_fw(struct adj_mat* adj_mat) {
    printf("doin' serial shit\n");
    printf("n: %d\n", adj_mat->n);
}
