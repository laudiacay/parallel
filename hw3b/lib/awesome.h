#ifndef _AWESOME_H
#define _AWESOME_H

#include "../provided/lib/packetsource.h"

void awesome_exp(PacketSource_t* packetsource,
    volatile Packet_t* (*getPacket)(PacketSource_t*, int),
    long m, int n, int D, char L);

#endif
