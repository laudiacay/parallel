import subprocess as sp

BIG = 10000000

def run_locks(t, l, n, b):
    run = sp.Popen(['../locks', str(t), str(l), str(n), str(BIG)],
        stdout=sp.PIPE
    )
    final_count = int(run.stdout.readline().strip())
    assert(final_count == BIG)
    time = float(run.stdout.readline().strip())
    return time

def run_locks_k_times(arg_tup, k):
    li = [run_locks(*arg_tup) for _ in range(k)]
    return sum(li)/k

throughput = {}

throughput['serial'] = BIG/run_locks_k_times(('s', 't', 1, BIG), 10)
throughput['taslock'] = BIG/run_locks_k_times(('p', 't', 1, BIG), 10)
throughput['ttaslock'] = BIG/run_locks_k_times(('p', '2', 1, BIG), 10)
throughput['mutex'] = BIG/run_locks_k_times(('p', 'm', 1, BIG), 10)
throughput['alock'] = BIG/run_locks_k_times(('p', 'a', 1, BIG), 10)
throughput['clhlock'] = BIG/run_locks_k_times(('p', 'c', 1, BIG), 10)

for k, v in throughput.items():
    print(f'{k}, {v}')

'''
want one csv row to look like

w   n=2   n=....n=9
'''
