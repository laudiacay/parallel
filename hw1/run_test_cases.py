import subprocess as sp
import filecmp

def test_n(n, t):
    s_out = 'serial_out.txt'
    p_out = 'parallel_out.txt'
    for p in range(1, 20):
        filename = 'tests/{}_nodes_{}_edgeprob.txt'.format(n, p)
        proc = sp.run(['./floydwarshall', 's', str(t), filename, s_out], stdout=sp.PIPE)
        proc = sp.run(['./floydwarshall', 'p', str(t), filename, p_out], stdout=sp.PIPE)
        assert(filecmp.cmp(s_out, p_out))

for n in [16, 32, 64, 128, 256, 512, 1024]:
    for t in [1, 2, 4, 8, 16, 32, 64]:
        if t <= n:
            print("testing n={}, t={}".format(n, t))
            test_n(n, t)
