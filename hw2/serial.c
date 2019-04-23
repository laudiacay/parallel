#include "lib/serial.h"

void serial_experiment(PacketSource_t* packetsource, long T, char mode) {

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
        for (int s_num = 0; s_num < packetsource->numSources; s_num++) {
            packet = getPacket(packetsource, s_num);
            // throwing out the checksum, might use later
            getFingerprint(packet->iterations, packet->seed);
            // other threads are definitely done w/ packet at this point
            // so we can cast away the volatile and free it
            free((void*) packet);
        }
    }

    // check that counts are the same as the number of packets?
    /*for (int s_num = 0; s_num < packetsource->numSources; s_num++){
        assert(getCount(packetsource, s_num) == T);
    }*/
}
