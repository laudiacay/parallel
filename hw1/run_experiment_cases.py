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
    print('{}, {}, {}, {}'.format(n, t, avg_speedup, var_speedup))

print('n, t, speedupmean, speedupvariance')
for n in [16, 32, 64, 128, 256, 512, 1024]:
    for t in [1, 2, 4, 8, 16, 32, 64]:
        if t <= n:
            test_n(n, t)
