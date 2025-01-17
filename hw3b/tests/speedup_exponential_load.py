import subprocess as sp

def run_packets(opt, m, n, W, pd, D, L, sd=0):
    try:
        run = sp.run(['../packets', str(opt), str(m), str(n), str(W), str(pd),
                                str(sd), str(D), str(L)],
            timeout=m/1000 + 3, stdout=sp.PIPE, stderr=sp.PIPE
        )
    except sp.TimeoutExpired:
        return run_packets(opt, m, n, W, pd, D, L, sd=sd)
    throughput = float(run.stdout.rstrip())
    return throughput

def run_packets_n_times(arg_tup, n):
    li = [run_packets(*arg_tup, sd=i) for i in range(n)]
    return sum(li)/n

m = 2000
#m = 10
n_opts = [1, 2, 3, 7, 13, 27]
#n_opts = [1, 2, 3]
L_opts = ['m', 'a']
W_opts = [1000, 2000, 4000, 8000]
#W_opts = [400, 800]
pd = 'e'
D = 8
result_dict = {}
opt = 'h'
for L in L_opts:
    for W in W_opts:
        for n in n_opts:
            result_dict[(opt, L, W, n)] = run_packets_n_times((opt, m, n, W, pd, D, L), 11)
opt = 'a'
for L in L_opts:
    for W in W_opts:
        for n in n_opts:
            result_dict[(opt, L, W, n)] = run_packets_n_times((opt, m, n, W, pd, D, L), 11)
L = 'm'
for W in W_opts:
    for n in n_opts:
        result_dict[('l', L, W, n)] = run_packets_n_times(('l', m, n, W, pd, D, L), 11)
        result_dict[('s', L, W, n)] = run_packets_n_times(('s', m, n, W, pd, D, L), 11)

for k, v in result_dict.items():
    print('u,'+','.join([str(i) for i in list(k)]) +','+str(v))
