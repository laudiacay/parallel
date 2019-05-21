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
            #print('done with {}'.format((opt, L, W)))
            #result_dict[(opt, L, W, n)] = run_packets_n_times((opt, m, n, W, pd, D, L), 1)
            result_dict[(opt, L, W, n)] = run_packets_n_times((opt, m, n, W, pd, D, L), 11)
L = 'm'
for W in W_opts:
    for n in n_opts:
        #print('done with {}'.format((opt, L, W)))
        #result_dict[('l', L, W, n)] = run_packets_n_times(('l', m, n, W, pd, D, L), 1)
        #result_dict[('s', L, W, n)] = run_packets_n_times(('s', m, n, W, pd, D, L), 1)
        result_dict[('l', L, W, n)] = run_packets_n_times(('l', m, n, W, pd, D, L), 11)
        result_dict[('s', L, W, n)] = run_packets_n_times(('s', m, n, W, pd, D, L), 11)

for W in W_opts:
    print("W = {}".format(W))
    print(',l,hm,ha')
    for n in n_opts:
        l_speedup = result_dict[('s', 'm', W, n)] / result_dict[('l', 'm', W, n)]
        hm_speedup = result_dict[('s', 'm', W, n)] / result_dict[('h', 'm', W, n)]
        ha_speedup = result_dict[('s', 'm', W, n)] / result_dict[('h', 'a', W, n)]
        print(str(n) + ',' + str(l_speedup) + ',' + str(hm_speedup) + ',' + str(ha_speedup))
