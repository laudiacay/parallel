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

W_opts = [1000, 2000, 4000, 8000]
n_opts = [2, 3, 5, 9, 14, 28]

nW_combs = [(n, W) for n in n_opts for W in W_opts]

serial_dict = {}
for i, (n, W) in enumerate(nW_combs):
    T = 2**17
    serial_dict[(n, W)] = run_packets_n_times(('s', pd, n, T, D, W, i), 5)

#print(serial_result_dict)

parallel_dict = {}
for i, (n, W) in enumerate(nW_combs):
    T = 2**17
    parallel_dict[(n, W)] = run_packets_n_times(('p', pd, n, T, D, W, i), 5)

#print(squeue_result_dict)

for n in n_opts:
    row_str = str(n) + ', '
    for W in W_opts:
        row_str += str(serial_dict[(n, W)]/parallel_dict[(n, W)])
        row_str += ', '
    print(row_str)

'''
want one csv row to look like

w   n=2   n=....n=9
'''
