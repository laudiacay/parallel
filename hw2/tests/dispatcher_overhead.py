import subprocess as sp

def run_packets(opt, pd, n, T, D, W, sd):
    run = sp.Popen(['../packets', str(opt), str(pd), str(n), str(T), str(D), str(W), str(sd)],
        stdout=sp.PIPE
    )
    time = float(run.stdout.readline().strip())
    return time

def run_packets_n_times(arg_tup, n):
    li = [run_packets(*arg_tup) for _ in range(n)]
    return sum(li)/n

D = 32
pd = 'u'

W = 1
n_opts = [2, 3, 5, 9, 14, 28]

dispatcher_results = {}
for i, n in enumerate(n_opts):
    T = (2**20)/(n - 1)
    dispatcher_results[n] = run_packets_n_times(('p', pd, n, T, D, W, i), 5)

for n in n_opts:
    print(n, ',', dispatcher_results[n])
