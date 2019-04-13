import subprocess as sp

def test_n(n, t):
    outfile = '/dev/null'
    speedups = []
    for p in range(1, 20):
        filename = 'tests/{}_nodes_{}_edgeprob.txt'.format(n, p)
        proc = sp.run(['./floydwarshall', 's', str(t), filename, outfile],
            stdout=sp.PIPE)
        s_time = float(proc.stdout.strip())
        proc = sp.run(['./floydwarshall', 'p', str(t), filename, outfile],
            stdout=sp.PIPE)
        p_time = float(proc.stdout.strip())
        speedups.append(s_time/p_time)
    avg_speedup = sum(speedups)/len(speedups)
    var_speedup = sum([(s - avg_speedup)**2 for s in speedups])/len(speedups)
    return avg_speedup, var_speedup

print('n, 1, 2, 4, 8, 16, 32, 64')
n_list = [16, 32, 64, 128, 256, 512, 1024]
t_list = [1, 2, 4, 8, 16, 32, 64]
lookups = {}
for n in n_list:
    for t in t_list:
        if t <= n:
            lookups[(n, t)] = test_n(n, t)

line = lambda n: str(n) + ', ' + ', '.join([str(lookups[(n,t)][0] if (n,t) in lookups else '') for t in t_list])
for n in n_list:
    print(line(n))
