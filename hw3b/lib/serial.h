#ifndef _SERIAL_H
#define _SERIAL_H

#include "../provided/lib/packetsource.h"

void serial_exp(PacketSource_t* packetsource,
    volatile Packet_t* (*getPacket)(PacketSource_t*, int),
    long m);

#endif
