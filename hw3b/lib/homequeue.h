#ifndef _HOMEQUEUE_H
#define _HOMEQUEUE_H

#include "../provided/lib/packetsource.h"

void homequeue_exp(PacketSource_t* packetsource,
    volatile Packet_t* (*getPacket)(PacketSource_t*, int),
    long m, int n, int D, char L);

#endif
