import subprocess as sp
import statistics

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
    return statistics.median(li)


