import subprocess as sp

def test_n(n, t):
    outfile = '/dev/null'
    serial_times = []
    parallel_times = []
    for p in range(1, 20):
        filename = 'tests/{}_nodes_{}_edgeprob.txt'.format(n, p)
        proc = sp.run(['./floydwarshall', 's', str(t), filename, outfile],
            stdout=sp.PIPE)
        serial_times.append(float(proc.stdout.strip()))
        proc = sp.run(['./floydwarshall', 'p', str(t), filename, outfile],
            stdout=sp.PIPE)
        parallel_times.append(float(proc.stdout.strip()))
    avg_serial = sum(serial_times)/len(serial_times)
    avg_parallel = sum(parallel_times)/len(parallel_times)
    print('{}, {}, {}, {}'.format(n, t, avg_serial, avg_parallel))

for n in [16, 32, 64, 128, 256, 512, 1024]:
    for t in [1, 2, 4, 8, 16, 32, 64]:
        if t <= n:
            test_n(n, t)
