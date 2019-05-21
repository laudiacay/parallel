#include <stdio.h>

#include "provided/lib/packetsource.h"

#define UNUSED(x) (void)(x)

void awesome_exp(PacketSource_t* packetsource,
    volatile Packet_t* (*getPacket)(PacketSource_t*, int),
    long m, int n, long W, char pd, short sd, int D, char L) {

    printf("doing awesome experiment\n");

    UNUSED(packetsource);
    UNUSED(getPacket);
    UNUSED(m);
    UNUSED(n);
    UNUSED(W);
    UNUSED(pd);
    UNUSED(sd);
    UNUSED(D);
    UNUSED(L);
}
