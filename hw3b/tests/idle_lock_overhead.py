import subprocess as sp

def run_packets(opt, m, n, W, pd, D, L, sd=0):
    run = sp.Popen(['../packets', str(opt), str(m), str(n), str(W), str(pd),
                                str(sd), str(D), str(L)],
        stdout=sp.PIPE
    )
    throughput = float(run.stdout.readline().strip())
    return throughput

def run_packets_n_times(arg_tup, n):
    li = [run_packets(*arg_tup, sd=i) for i in range(n)]
    return sum(li)/n

m = 10000
#m = 10
n = 1
opt_opts = ['l', 'h']
L_opts = ['m', 'a']
W_opts = [25, 50, 100, 200, 400, 800]
#W_opts = [400, 800]
pd = 'u'
D = 8
result_dict = {}
for opt in opt_opts:
    for L in L_opts:
        for W in W_opts:
            #print('done with {}'.format((opt, L, W)))
            #result_dict[(opt, L, W)] = run_packets_n_times((opt, m, n, W, pd, D, L), 1)
            result_dict[(opt, L, W)] = run_packets_n_times((opt, m, n, W, pd, D, L), 5)

print('l,'+','.join([str(W) for W in W_opts]))

for L in L_opts:
    print(str(L) + ',' + ','.join([str(result_dict[('h', L, W)]/result_dict[('l',L,W)]) for W in W_opts]))
