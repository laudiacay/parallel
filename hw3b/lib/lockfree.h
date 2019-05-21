#ifndef _LOCKFREE_H
#define _LOCKFREE_H

#include "../provided/lib/packetsource.h"

void lockfree_exp(PacketSource_t* p_source,
    volatile Packet_t* (*getPacket)(PacketSource_t*, int),
    long m, int n, int D);

#endif
