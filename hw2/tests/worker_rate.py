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

W_opts = [25, 50, 100, 200, 400, 800]
n_opts = [2, 9, 14]

nW_combs = [(n, W) for n in n_opts for W in W_opts]

squeue_result_dict = {}
for i, (n, W) in enumerate(nW_combs):
    T = (2**24)/(n*W)
    squeue_result_dict[(n, W)] = run_packets_n_times(('q', pd, n, T, D, W, i), 5)

for W in W_opts:
    row_str = str(W) + ', '
    for n in n_opts:
        row_str += str(squeue_result_dict[(n, W)])
        row_str += ', '
    print(row_str)

'''
want one csv row to look like

w   n=2   n=....n=9
'''
