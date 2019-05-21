#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "lib/serial.h"
#include "lib/lockfree.h"
#include "lib/homequeue.h"
#include "lib/awesome.h"

#include "provided/lib/packetsource.h"

int usage() {
    printf("Usage:\n");
    printf("\t   Serial: ./packets s m n W pd sd D L\n");
    printf("\t LockFree: ./packets l m n W pd sd D L\n");
    printf("\tHomeQueue: ./packets h m n W pd sd D L\n");
    printf("\t  Awesome: ./packets a m n W pd sd D L\n");
    printf(" m: # milliseconds that experiment should run\n");
    printf(" n: number of sources\n");
    printf(" W: mean work per packet\n");
    printf("pd: u=uniform, e=exponential\n");
    printf("sd: seed\n");
    printf(" D: queue depth\n");
    printf(" L: lock algorithm, m=mutex/pthread, a=array lock\n");
    return 1;
}

void error_and_exit() {
    printf("An error occurred!\n");
    exit(1);
}

int main(int argc, char *argv[]) {

    if (argc != 9) return usage();

    long m;
    int n;
    long W;
    char pd = argv[5][0];
    short sd;
    int D;
    char L = argv[8][0];

    if (sscanf(argv[2], "%ld", &m) != 1) return usage();
    if (sscanf(argv[3], "%d", &n) != 1) return usage();
    if (sscanf(argv[4], "%ld", &W) != 1) return usage();
    if (sscanf(argv[6], "%hd", &sd) != 1) return usage();
    if (sscanf(argv[7], "%d", &D) != 1) return usage();

    PacketSource_t* p_source = createPacketSource(W, n, sd);

    volatile Packet_t* (*getPacket)(PacketSource_t*, int);
    long (*getCount)(PacketSource_t*, int);

    switch (pd) {
        case 'u':
            getPacket = getUniformPacket;
            getCount = getUniformCount;
            break;
        case 'e':
            getPacket = getExponentialPacket;
            getCount = getExponentialCount;
            break;
        default:
            deletePacketSource(p_source);
            return usage();
    }

    switch (argv[1][0]) {
        case 's':
            serial_exp(p_source, getPacket, m);
            break;
        case 'l':
            lockfree_exp(p_source, getPacket, m, n, D);
            break;
        case 'h':
            homequeue_exp(p_source, getPacket, m, n, D, L);
            break;
        case 'a':
            awesome_exp(p_source, getPacket, m, n, D, L);
            break;
        default:
            deletePacketSource(p_source);
            return usage();
    }

    long packet_total = 0;
    for (int s_num = 0; s_num < p_source->numSources; s_num++){
        packet_total += getCount(p_source, s_num);
    }

    deletePacketSource(p_source);

    // print throughput in packets per second
    printf("%f\n", (float)packet_total / (float)W * 1000);
    return 0;
}
