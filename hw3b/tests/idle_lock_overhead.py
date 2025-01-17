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
n = 1
opt_opts = ['l', 'h']
L_opts = ['m', 'a']
W_opts = [25, 50, 100, 200, 400, 800]
pd = 'u'
D = 8
result_dict = {}
for opt in opt_opts:
    for L in L_opts:
        for W in W_opts:
            #print('done with {}'.format((opt, L, W)))
            #result_dict[(opt, L, W)] = run_packets_n_times((opt, m, n, W, pd, D, L), 1)
            result_dict[(opt, L, W)] = run_packets_n_times((opt, m, n, W, pd, D, L), 5)

for k, v in result_dict.items():
    print('u,'+','.join([str(i) for i in list(k)]) +','+str(v))
