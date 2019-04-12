# python script to generate test cases of size n and put them in tests
# uses erdos-renyi binomial model to generate graphs with different sparsity

import random

INF = 10000000

# n is number of nodes, p is probability of including an edge
# weights are uniformly chosen from 1 to 1000 inclusive
def gen_graph(n, p):
    adj_mat = [[INF if i != j else 0 for i in range(n)] for j in range(n)]
    for i in range(n):
        for j in range(n):
            if i == j: continue
            if random.random() < p:
                adj_mat[i][j] = random.randint(1, 1000)
    return adj_mat

def write_graph(outfile, adj_mat):
    with open(outfile, 'w+') as f:
        f.write(str(len(adj_mat)) + '\n')
        for row in adj_mat:
            for col in row:
                f.write(str(col) + ' ')
            f.write('\n')

# generates very sparse to nearly fully connected!
def gen_and_write_graphs_for_n(n):
    for p in range(1, 20):
        adj_mat = gen_graph(n, p/20)
        filename = 'tests/{}_nodes_{}_edgeprob.txt'.format(n, p)
        write_graph(filename, adj_mat)

for n in [16, 32, 64, 128, 256, 512, 1024]:
    print('generating n:', n)
    gen_and_write_graphs_for_n(n)
