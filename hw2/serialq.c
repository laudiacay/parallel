#include "lib/serialq.h"

void serialq_experiment(PacketSource_t* packetsource, long T, int n, char mode,
        struct WaitFreeQueue** qs) {
    volatile Packet_t* (*getPacket)(PacketSource_t*, int);
    //long (*getCount)(PacketSource_t*, int);

    switch (mode) {
        case 'u':
            getPacket = getUniformPacket;
            //getCount = getUniformCount;
            break;
        case 'e':
            getPacket = getExponentialPacket;
            //getCount = getExponentialCount;
            break;
        case 'c':
            getPacket = getConstantPacket;
            //getCount = getConstantCount;
            break;
        default:
            exit(0);
    }

    volatile Packet_t* packet;
    // iterate over number of packets per source
    for (long p_num = 0; p_num < T; p_num++) {
        // iterate over all sources

        // enqueue packets
        for (int i = 0; i < n; i++) {
            assert(!enq(qs[i], getPacket(packetsource, i)));
        }

        // dequeue and process packets
        for (int i = 0; i < n; i++) {
            while (! (packet = (volatile Packet_t*) deq(qs[i])));
            // throwing out the checksum, might use later
            getFingerprint(packet->iterations, packet->seed);
            // other threads are definitely done w/ packet at this point
            // so we can cast away the volatile and free it
            free((void*) packet);
        }
    }

}
